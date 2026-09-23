#include "fixation_monitor.h"

#include <algorithm>
#include <chrono>
#include <QFileInfo>
#include <thread>

#include <QCoreApplication>
#include <QStringList>
#include <opencv2/imgproc.hpp>

#include "qt/qt_interop.h"

namespace {

// 依次试 cwd、可执行文件上一级、上两级三个候选路径，返回第一个存在的文件，都找不到返回空串。
// 调用者：FixationMonitor::start()（GUI 线程）。之所以要多级回退，是因为程序可能从 lab8/
// 或 lab8/build/ 启动，而模型文件固定在仓库根的 yunet_trt_cpp/models/ 下。
QString findModel(const QString &relative)
{
    const QStringList candidates{
        relative,
        QCoreApplication::applicationDirPath() + "/../" + relative,
        QCoreApplication::applicationDirPath() + "/../../" + relative};
    for (const QString &candidate : candidates)
        if (QFileInfo(candidate).isFile())
            return candidate;
    return {};
}

// 把 YuNet 输出的浮点矩形截断成整型，再与画面尺寸求交，保证 ROI 完全落在画面内。
// 调用者：FixationMonitor::loop()（工作线程）。必须先裁剪，是因为人脸/眼框位于画面边缘时可能越界，
// 越界矩形传给 cvtColor 或用作 cv::Mat 子区都会抛异常。
cv::Rect clippedRect(const cv::Rect2f &rect, const cv::Size &size)
{
    return cv::Rect(static_cast<int>(rect.x), static_cast<int>(rect.y),
                    static_cast<int>(rect.width), static_cast<int>(rect.height))
        & cv::Rect(0, 0, size.width, size.height);
}

// 在眼 ROI 内定位瞳孔：转灰度 → GaussianBlur(5,5) 去噪 → 按阈值 55 反转二值化 →
// 取最大轮廓 → 以其包围盒中心作为瞳孔位置，换算成全图坐标写入 *center，返回是否找到。
// 调用者：FixationMonitor::loop()（工作线程）。用 THRESH_BINARY_INV 是因为瞳孔是暗区，
// 反转后才成为白色前景供 findContours 搜索；取最大轮廓是因为瞳孔就是 ROI 内面积最大的暗区。
// 阈值 55 是硬编码常量（与 lab6 的瞳孔检测同构）。
bool findPupil(const cv::Mat &frame, const cv::Rect &roi, cv::Point *center)
{
    if (!center || roi.empty())
        return false;
    cv::Mat gray;
    if (frame.channels() == 1)
        gray = frame(roi);
    else if (frame.channels() == 3)
        cv::cvtColor(frame(roi), gray, cv::COLOR_BGR2GRAY);
    else if (frame.channels() == 4)
        cv::cvtColor(frame(roi), gray, cv::COLOR_BGRA2GRAY);
    else
        return false;

    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0.0);
    cv::Mat binary;
    cv::threshold(gray, binary, 55, 255, cv::THRESH_BINARY_INV);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty())
        return false;
    const auto largest = std::max_element(contours.begin(), contours.end(),
        [](const auto &left, const auto &right) {
            return cv::contourArea(left) < cv::contourArea(right);
        });
    const cv::Rect bounds = cv::boundingRect(*largest);
    if (bounds.area() <= 0)
        return false;
    *center = roi.tl() + bounds.tl() + cv::Point(bounds.width / 2, bounds.height / 2);
    return true;
}

}

// 由 MainWindow 在 GUI 线程创建并亲和 GUI 线程（本类没有定时器），这里只转交 QObject 构造。
FixationMonitor::FixationMonitor(QObject *parent) : QObject(parent) {}

// 析构前先 stop()：join 工作线程并释放 m_source/m_detector，避免线程继续访问正在销毁的成员。
FixationMonitor::~FixationMonitor() { stop(); }

// GUI 线程（MainWindow 的按钮 clicked 直接触发）。m_running 守卫防重复启动，随后依次找模型、
// 初始化 YuNet、打开摄像头、复位自标定状态，最后起工作线程跑 loop()。
void FixationMonitor::start()
{
    if (m_running.load())
        return;

    const QString onnx = findModel("yunet_trt_cpp/models/face_detection_yunet_2023mar.onnx");
    const QString engine = findModel("yunet_trt_cpp/models/yunet_fp16.engine");
    if (onnx.isEmpty() || engine.isEmpty()) {
        emit errorOccurred(tr("找不到 YuNet ONNX 或 TensorRT engine"));
        return;
    }

    try {
        m_detector = std::make_unique<YuNetTRT>();
        // 参数为 useFp16=true、forceRebuild=false：优先加载已序列化的 engine，没有才从 ONNX 重建。
        if (!m_detector->init(onnx.toStdString(), engine.toStdString(), true, false)) {
            emit errorOccurred(tr("YuNet TensorRT 初始化失败"));
            m_detector.reset();
            return;
        }
    } catch (const std::exception &error) {
        emit errorOccurred(QString::fromLocal8Bit(error.what()));
        m_detector.reset();
        return;
    }

    if (!m_source.open()) {
        emit errorOccurred(m_source.lastError());
        m_detector.reset();
        return;
    }

    {
        std::lock_guard<std::mutex> lock(m_stateMutex);
        // 每次启动都重新自标定：m_haveCentre 置假后，工作线程第一帧"两眼瞳孔都检测到"的画面会重设基线。
        m_haveCentre = false;
        // 本次启动尚未发布过任何注视状态，直接复位即可，不需要补发信号。
        m_lastValid = false;
    }
    m_stopRequested.store(false);
    m_running.store(true);
    // 这里直接赋值而未经 join：正常路径由 stop() 保证线程已结束，但若工作线程因摄像头出错自行
    // break 退出，m_thread 此时仍是 joinable，再次点击启动会 move-assign 到 joinable 的 std::thread。
    m_thread = std::thread(&FixationMonitor::loop, this);
}

// GUI 线程（按钮或析构）直接调用：置停止标志后 join 工作线程，界面最多阻塞一个检测周期。
// 若停止前处于注视有效状态，补发一次 fixationChanged(false)，让 Amsler 侧转入 WaitingFixation；
// 否则状态机会一直以为注视仍然有效。
void FixationMonitor::stop()
{
    m_stopRequested.store(true);
    if (m_thread.joinable())
        m_thread.join();
    m_source.release();
    m_detector.reset();
    m_running.store(false);

    bool emitInvalid = false;
    {
        std::lock_guard<std::mutex> lock(m_stateMutex);
        emitInvalid = m_lastValid;
        m_lastValid = false;
        m_haveCentre = false;
    }
    if (emitInvalid)
        emit fixationChanged(false);
}

// 工作线程主循环，33ms 一拍，直到 m_stopRequested 被置起。
// 注视判定链：取帧 → clone 出可画的画布 → YuNet 检测（topK=20）→ 取 score 最高的脸 →
// computeEyeRois 推算双眼 ROI → 每个 ROI 先裁剪再用 findPupil 找瞳孔 → 两眼都找到时取两瞳孔中点 →
// 与自标定基线比较，距离 < 22px 判为注视有效。
void FixationMonitor::loop()
{
    while (!m_stopRequested.load()) {
        const auto frame = m_source.next();
        if (!frame || frame->mat.empty()) {
            // 若是 stop() 主动要求退出导致的取帧失败就不再报错，否则报错后退出循环。
            if (!m_stopRequested.load())
                emit errorOccurred(tr("摄像头读取失败"));
            break;
        }

        bool valid = false;
        // 原帧是共享的 const 帧，不能在上面画标注，因此深拷贝一份作为绘制画布。
        cv::Mat canvas = frame->mat.clone();
        try {
            // topK 取 20 而非 lab7 的 5000：镜头里通常只有一两张脸，小 topK 可省去大量后处理；
            // score/nms 阈值与 lab7 一致（0.6 / 0.3）。
            const std::vector<FaceResult> faces = m_detector->detect(frame->mat, 0.6f, 0.3f, 20);
            if (!faces.empty()) {
                // 多张脸时只认 score 最高的那张，避免旁人误入镜头就改变注视判定。
                const FaceResult &face = *std::max_element(
                    faces.begin(), faces.end(), [](const FaceResult &left, const FaceResult &right) {
                        return left.score < right.score;
                    });
                const auto eyes = computeEyeRois(face, 0.6f);
                std::array<cv::Point, 2> pupils{};
                bool bothFound = true;
                for (int index = 0; index < 2; ++index) {
                    // index 0 = 右眼（画蓝框），index 1 = 左眼（画红框），与 lab7 的标注约定一致。
                    // ROI 先与画面求交，越界的子区会让 findPupil 里的 cvtColor 抛异常。
                    const cv::Rect roi = clippedRect(eyes[static_cast<std::size_t>(index)], frame->mat.size());
                    if (roi.empty()) { bothFound = false; continue; }
                    cv::rectangle(canvas, roi, index == 0 ? cv::Scalar(255, 0, 0) : cv::Scalar(0, 0, 255), 2);
                    if (!findPupil(frame->mat, roi, &pupils[static_cast<std::size_t>(index)])) {
                        bothFound = false;
                        continue;
                    }
                    cv::drawMarker(canvas, pupils[static_cast<std::size_t>(index)], cv::Scalar(0, 255, 255), cv::MARKER_CROSS, 10, 2);
                }
                if (bothFound) {
                    // 两眼瞳孔都找到才判定：单眼结果不稳，取中点可抵消头部轻微转动。
                    const cv::Point current((pupils[0].x + pupils[1].x) / 2, (pupils[0].y + pupils[1].y) / 2);
                    // m_centre/m_haveCentre 会被 GUI 线程的 start()/stop() 读写，故与它们统一加锁。
                    std::lock_guard<std::mutex> lock(m_stateMutex);
                    // m_haveCentre 为假即还没自标定：把这一帧当作基线（此刻距离为 0，必然判为有效）。
                    // 基线之后不再更新，也没有重新标定的入口，只能靠 stop()/start() 重置。
                    if (!m_haveCentre) { m_centre = current; m_haveCentre = true; }
                    // 22px 是硬编码容差：瞳孔中点偏离基线超过它就算视线移开。
                    valid = cv::norm(current - m_centre) < 22.0;
                }
                const cv::Rect faceRect = clippedRect(face.box, frame->mat.size());
                cv::rectangle(canvas, faceRect, cv::Scalar(0, 255, 0), 2);
            }
        } catch (const std::exception &error) {
            // 推理抛异常后无法继续取结果，报错并结束线程。
            emit errorOccurred(QString::fromLocal8Bit(error.what()));
            break;
        }

        bool changed = false;
        {
            std::lock_guard<std::mutex> lock(m_stateMutex);
            // 边沿触发：只有判定结果翻转时才对外发信号，避免每帧都发让下游重复处理。
            changed = valid != m_lastValid;
            m_lastValid = valid;
        }
        if (changed)
            emit fixationChanged(valid);
        // 与 lab7 不同：本类没有 QTimer，工作线程直接 emit，靠 Qt 队列连接投递到 GUI 线程；
        // canvas 已深拷贝、toQImage 也生成独立的 QImage 缓冲，因此跨线程传递是安全的。
        emit frameChanged(labqt::toQImage(canvas));
        // 约 30Hz 的节拍，限制检测频率并让出 CPU。
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    // 循环退出（正常停止或出错）时清运行标志，使 active() 立即变假。
    m_running.store(false);
}

#include "detector_worker.h"

#include <algorithm>
#include <chrono>
#include <thread>

#include <QCoreApplication>
#include <QFileInfo>          // ← 加这个
#include <QTimer>
#include <opencv2/imgproc.hpp>

#include "qt/qt_interop.h"

namespace {

// 匿名命名空间里的辅助函数，只在 start() 里（主线程）被调用两次：按三级候选目录依次回退定位模型文件，
// 依次是当前工作目录、可执行文件所在目录的上一级、上两级。
// 之所以要三级回退：按 build 到 lab7/build/ 的方式运行时可执行文件在 lab7/build/，模型却在仓库根的
// yunet_trt_cpp/models/，只有第三个候选能命中；若在仓库根直接运行 lab7/build/lab7，则第一个候选就命中。
// 命中即返回，三级都不命中返回空串。
QString findPath(const QString &relativePath)
{
    const QStringList candidates{
        relativePath,
        QCoreApplication::applicationDirPath() + "/../" + relativePath,
        QCoreApplication::applicationDirPath() + "/../../" + relativePath};
    for (const QString &candidate : candidates)
        if (QFileInfo(candidate).isFile())   // ← 用 QFileInfo 判断
            return candidate;
    return QString();
}

}

// 构造函数在主线程执行（由主窗口的构造函数创建）：DetectorWorker 虽是 QObject，但亲和创建它的线程，
// 所以它自己的 QTimer 的 timeout 也在主线程触发，而不是在工作线程。
DetectorWorker::DetectorWorker(QObject *parent)
    : QObject(parent)
{
    // 33ms ≈ 30Hz，与工作线程 20ms 的产帧节奏刻意不同步：取不到新帧就跳过，多余的帧被静默丢弃。
    m_timer = new QTimer(this);
    m_timer->setInterval(33);
    // 主线程定时器负责把工作线程写入的最新帧取出来后再 emit，QPixmap/QLabel 因此只在 GUI 线程被访问。
    connect(m_timer, &QTimer::timeout, this, [this] {
        QImage image;
        {
            std::lock_guard<std::mutex> lock(m_imageMutex);
            image = std::move(m_latest);
        }
        if (!image.isNull())
            emit frameChanged(image);
    });
}

// 析构在主线程执行：兜底 join 工作线程并停表，避免线程继续访问正在析构的成员。
DetectorWorker::~DetectorWorker()
{
    stop();
}

// 由主线程调用（主窗口里是「开始检测」按钮的直连槽）：找模型路径、初始化 TensorRT、开摄像头并起工作线程。
void DetectorWorker::start()
{
    if (running())
        return;
    const QString onnx = findPath("yunet_trt_cpp/models/face_detection_yunet_2023mar.onnx");
    const QString engine = findPath("yunet_trt_cpp/models/yunet_fp16.engine");
    if (onnx.isEmpty() || engine.isEmpty()) {
        emit errorOccurred(tr("找不到 YuNet ONNX 或 TensorRT engine"));
        return;
    }
    // init 的第三个参数 useFp16=true、第四个 forceRebuild=false 即「先试加载、失败才重建」策略：
    // 优先加载已序列化的 engine，只有 engine 缺失或不可用时才回退到用 ONNX 重建并写盘，
    // 省掉每次启动都重建引擎的开销；engine 不能跨 TensorRT 版本或 GPU 架构复用，换机需重建。
    try {
        m_detector = std::make_unique<YuNetTRT>();
        if (!m_detector->init(onnx.toStdString(), engine.toStdString(), true, false)) {
            emit errorOccurred(tr("TensorRT YuNet 初始化失败"));
            m_detector.reset();
            return;
        }
    } catch (const std::exception &error) {
        // TensorRT/CUDA 的失败以异常抛出，这里统一转成 UI 提示，不让异常穿出槽函数。
        emit errorOccurred(QString::fromLocal8Bit(error.what()));
        m_detector.reset();
        return;
    }
    // init 失败的两个分支都已 reset 并提前 return，能走到这里 m_detector 必然非空，此判断实际不可达。
    if (!m_detector) {
        emit errorOccurred(tr("YuNet CPU 检测器创建失败"));
        return;
    }
    // 摄像头在主线程打开，打开之后的 next() 取帧全部发生在工作线程 loop() 里。
    if (!m_source.open()) {
        emit errorOccurred(m_source.lastError());
        return;
    }
    m_stopRequested.store(false);
    m_running.store(true);
    m_timer->start();
    // 只有 loop() 跑在工作线程，start()/stop() 自身仍留在主线程执行。
    // 注意：若上一轮 loop() 因出错自行结束，m_thread 仍是 joinable 状态，此时再点「开始检测」
    // 会对 joinable 的 std::thread 做 move 赋值。
    m_thread = std::thread(&DetectorWorker::loop, this);
    emit statusChanged(tr("YuNet TensorRT 检测运行中，%1").arg(m_source.description()));
}

// 由主线程调用：先置停止标志让 loop() 退出，再 join（界面因此最多阻塞一个检测周期），最后释放摄像头与检测器。
void DetectorWorker::stop()
{
    // 先置标志再 join，保证 loop() 在下一轮循环条件处就能退出。
    m_stopRequested.store(true);
    if (m_thread.joinable())
        m_thread.join();
    // 释放顺序与 start() 相反：线程已停，再关摄像头，最后析构持有 GPU 资源的检测器。
    m_source.release();
    m_detector.reset();
    m_running.store(false);
    if (m_timer)
        m_timer->stop();
}

// 工作线程入口，只由 std::thread 调用：取帧 → 推理 → 画框 → 写 m_latest，直到 stop() 置位或出错。
// 正常帧通路不 emit 任何信号，只把图写进 m_latest（加锁），由主线程定时器取出后再转发；
// 只有错误路径会在这里 emit errorOccurred，靠队列连接投递回 GUI 线程。
void DetectorWorker::loop()
{
    while (!m_stopRequested.load()) {
        const auto frame = m_source.next();
        if (!frame || frame->mat.empty()) {
            // 取不到帧：若是 stop() 主动停止则静默退出，否则视为摄像头故障，报错后结束循环。
            if (!m_stopRequested.load())
                emit errorOccurred(tr("摄像头读取失败"));
            break;
        }
        try {
            // 三个推理参数都是编译期字面量，界面上没有对应控件：score=0.6f、NMS=0.3f、topK=5000。
            const std::vector<FaceResult> faces = m_detector->detect(frame->mat, 0.6f, 0.3f, 5000);
            const QImage image = annotate(frame->mat, faces);
            // 写入最新帧时加锁：主线程定时器可能正同时把 m_latest 取走。
            std::lock_guard<std::mutex> lock(m_imageMutex);
            m_latest = image;
        } catch (const std::exception &error) {
            emit errorOccurred(QString::fromLocal8Bit(error.what()));
            break;
        }
        // 20ms 一轮（≲50Hz），比 UI 的 33ms 快，中间产生的帧会被主线程静默跳过。
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    // 循环自行结束（出错 break）时也会清 m_running，但 m_thread 仍 joinable，要等下一次 stop() 才 join。
    m_running.store(false);
}

// 由工作线程的 loop() 调用（静态成员，不访问 this）：在帧的克隆副本上绘制检测结果，再转成 QImage。
// 几何：人脸框绿 2px；5 个关键点红色实心圆 r=3；眼框由 computeEyeRois 推算，索引 0=右眼、索引 1=左眼。
QImage DetectorWorker::annotate(const cv::Mat &frame, const std::vector<FaceResult> &faces)
{
    // clone 出一份再画，避免改动 loop() 里正在用的原帧。
    cv::Mat canvas = frame.clone();
    for (const FaceResult &detection : faces) {
        // box 与 landmarks 都是原图像素坐标的浮点值，转 int 后直接使用。
        const cv::Rect face(static_cast<int>(detection.box.x),
                            static_cast<int>(detection.box.y),
                            static_cast<int>(detection.box.width),
                            static_cast<int>(detection.box.height));
        cv::rectangle(canvas, face, cv::Scalar(0, 255, 0), 2);
        // YuNet 只输出人脸框和 5 个关键点（顺序：右眼、左眼、鼻尖、右嘴角、左嘴角），并不输出眼睛框。
        for (int point = 0; point < 5; ++point) {
            const cv::Point landmark(static_cast<int>(detection.landmarks[point].x),
                                     static_cast<int>(detection.landmarks[point].y));
            cv::circle(canvas, landmark, 3, cv::Scalar(0, 0, 255), cv::FILLED);
        }
        // 眼框是按关键点推算的：以该眼关键点为中心，边长 = 0.6 × 双眼关键点欧氏距离，0.6f 就是这个比例。
        // 这里算出的 eyeRois 只用于画框，lab7 中没有任何代码消费它，不做瞳孔搜索也不做测量。
        const std::array<cv::Rect2f, 2> eyeRois = computeEyeRois(detection, 0.6f);
        const cv::Rect right(static_cast<int>(eyeRois[0].x), static_cast<int>(eyeRois[0].y),
                             static_cast<int>(eyeRois[0].width), static_cast<int>(eyeRois[0].height));
        const cv::Rect left(static_cast<int>(eyeRois[1].x), static_cast<int>(eyeRois[1].y),
                            static_cast<int>(eyeRois[1].width), static_cast<int>(eyeRois[1].height));
        // 索引 0 = 右眼 → 蓝框 + 文字 "R"，索引 1 = 左眼 → 红框 + 文字 "L"；
        // 颜色以 BGR 给出，Scalar(255,0,0) 是蓝、Scalar(0,0,255) 是红，与关键点的红圆点区分开。
        cv::rectangle(canvas, right, cv::Scalar(255, 0, 0), 2);
        cv::rectangle(canvas, left, cv::Scalar(0, 0, 255), 2);
        cv::putText(canvas, "R", right.tl() + cv::Point(0, -4),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 0, 0), 2);
        cv::putText(canvas, "L", left.tl() + cv::Point(0, -4),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 2);
    }
    return labqt::toQImage(canvas);
}

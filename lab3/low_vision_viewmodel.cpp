#include "low_vision_viewmodel.h"

#include <algorithm>
#include <chrono>
#include <utility>

#include <QTimer>

#include "blur_two/blur_two.h"
#include "cataract/cataract.h"
#include "glare/glare.h"
#include "qt/qt_interop.h"
#include "visual_field_loss/visual_field_loss.h"

#include <opencv2/imgproc.hpp>

// 构造（GUI 线程）：建立刷新定时器。定时器只负责从单槽取帧，图像处理全在工作线程。
LowVisionViewModel::LowVisionViewModel(QObject *parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);
    // 33ms 与工作线程的生产节拍相同：两端节奏独立，谁慢都不会拖住对方，积压的旧帧直接覆盖丢弃。
    m_timer->setInterval(33);
    connect(m_timer, &QTimer::timeout, this, [this] {
        QImage image;
        {
            std::lock_guard<std::mutex> lock(m_imageMutex);
            image = std::move(m_latest);
        }
        // 先在锁内把帧取走，再在锁外 emit：避免持锁发出信号时被 GUI 槽重入阻塞。
        if (!image.isNull())
            emit frameChanged(image);
    });
}

// 析构：先 stop()，保证工作线程已 join 再销毁 QObject 成员，否则线程可能访问已销毁对象。
LowVisionViewModel::~LowVisionViewModel()
{
    stop();
}

// 摄像头模式的入口：转发到 start()，空路径表示非图片模式。
bool LowVisionViewModel::startCamera()
{
    return start(false, QString());
}

// 图片模式入口；路径为空时直接返回 false，不进入 start() 以免打开源必然失败。
bool LowVisionViewModel::openImage(const QString &path)
{
    return path.isEmpty() ? false : start(true, path);
}

// 起停的统一实现（GUI 线程）：先停旧任务保证同一时刻只有一个工作线程，再复位标志、起线程、开定时器。
bool LowVisionViewModel::start(bool imageMode, const QString &path)
{
    stop();
    {
        std::lock_guard<std::mutex> lock(m_imageMutex);
        // 清掉上一轮的残留帧，否则新一轮启动时会先闪一帧旧画面。
        m_latest = QImage();
    }
    // 先复位停止标志再置运行标志：工作线程一启动读到的 stop 必定已是 false。
    m_stopRequested.store(false);
    m_running.store(true);
    m_worker = std::thread(&LowVisionViewModel::workerLoop, this, imageMode, path);
    m_timer->start();
    emit stateChanged(imageMode ? QStringLiteral("图片处理中") : QStringLiteral("摄像头运行中"));
    return true;
}

// 停止（GUI 线程）：置停止标志并 join，界面最多阻塞一个 33ms 处理周期，之后源才可以安全释放。
void LowVisionViewModel::stop()
{
    m_stopRequested.store(true);
    if (m_worker.joinable())
        m_worker.join();
    m_source.release();
    m_running.store(false);
    if (m_timer)
        m_timer->stop();
    emit stateChanged(QStringLiteral("已停止"));
}

// 工作线程入口：打开源、循环取帧处理并发布。全程不碰 QWidget，emit 只依赖 Qt 队列连接投递到 GUI 线程。
void LowVisionViewModel::workerLoop(bool imageMode, QString path)
{
    std::string error;
    bool opened = false;
    if (imageMode) {
        opened = m_source.openImage(path);
    } else {
        opened = m_source.open();
    }
    if (!opened) {
        // 打开失败：报错并让线程自行退出；m_worker 仍处于 joinable 状态，靠下一次 start() 开头的 stop() 收尾。
        emit errorOccurred(m_source.lastError());
        m_running.store(false);
        return;
    }

    while (!m_stopRequested.load()) {
        const auto frame = m_source.next();
        if (!frame || frame->mat.empty()) {
            // 取不到帧即结束循环；主动 stop 触发的退出不算错误，故只在非停止状态下报错。
            if (!m_stopRequested.load())
                emit errorOccurred(m_source.lastError());
            break;
        }
        Params params;
        {
            std::lock_guard<std::mutex> lock(m_paramMutex);
            // 整份拷贝参数快照：既保证一帧内四个效果看到的是同一组值，也让算法执行期间不必持锁。
            params = m_params;
        }
        const cv::Mat output = process(frame->mat, params);
        // toQImage 会深拷贝像素，因此拷出来的 QImage 可以安全跨线程交给 GUI 定时器。
        publish(labqt::toQImage(output));
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    m_source.release();
    m_running.store(false);
}

// 逐帧处理链（工作线程）：顺序固定为 模糊 → 视野缺失 → 白内障 → 眩光，最后按需叠 Sobel。
// 每一级在对应 severity=0 时都是空操作（返回 clone），所以取消勾选不需要在这里加分支。
cv::Mat LowVisionViewModel::process(const cv::Mat &source, const Params &params) const
{
    // 先克隆一份：后续每一级都返回新矩阵，克隆保证这张临时图上不会改到调用方的帧。
    cv::Mat result = source.clone();
    result = lab3::blur_two::apply(result, params.blur);
    result = lab3::visual_field_loss::apply(result, params.fieldLoss);
    result = lab3::cataract::apply(result, params.cataract);
    result = lab3::glare::apply(result, params.glare);
    // Sobel 用原子量单独读，因此这里不需要锁；只有 lab4 的界面会把它置 true。
    if (m_sobelEnabled.load()) {
        cv::Mat gray;
        cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
        cv::Mat edges;
        // CV_16S 承接 Sobel 的负梯度与超 8 位值，convertScaleAbs 再取绝对值并饱和回 8U。
        cv::Sobel(gray, edges, CV_16S, 1, 0, 3);
        cv::convertScaleAbs(edges, result);
    }
    return result;
}

// 发布一帧到单槽（工作线程调用）：空帧直接丢弃，非空则覆盖旧帧，等待 GUI 定时器取走。
void LowVisionViewModel::publish(const QImage &image)
{
    if (image.isNull())
        return;
    std::lock_guard<std::mutex> lock(m_imageMutex);
    m_latest = image;
}

// 参数写入的公共实现（GUI 线程）：成员指针指定字段，四个 setXxx 共用；夹取范围与滑条量程一致。
void LowVisionViewModel::setParam(int Params::*member, int value)
{
    std::lock_guard<std::mutex> lock(m_paramMutex);
    this->m_params.*member = std::clamp(value, 0, 100);
}

void LowVisionViewModel::setBlur(int value) { setParam(&Params::blur, value); }
void LowVisionViewModel::setFieldLoss(int value) { setParam(&Params::fieldLoss, value); }
void LowVisionViewModel::setCataract(int value) { setParam(&Params::cataract, value); }
void LowVisionViewModel::setGlare(int value) { setParam(&Params::glare, value); }
void LowVisionViewModel::setSobel(bool enabled) { m_sobelEnabled.store(enabled); }

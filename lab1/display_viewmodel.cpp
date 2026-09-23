#include "display_viewmodel.h"

#include <chrono>
#include <optional>

#include <opencv2/imgcodecs.hpp>

#include <QTimer>

#include "qt/logging.h"
#include "qt/qt_interop.h"

// GUI 线程构造：建立刷新定时器，超时即回调 pullFrame（同线程直连，立即执行）。
DisplayViewModel::DisplayViewModel(QObject *parent)
    : QObject(parent)
{
    m_frameTimer = new QTimer(this);
    // 与采集端的 33ms 同频（约 30Hz）：两端同速，多余的帧由单槽自然丢弃。
    m_frameTimer->setInterval(33);
    connect(m_frameTimer, &QTimer::timeout, this, &DisplayViewModel::pullFrame);
}

// GUI 线程析构：停掉采集线程，避免工作线程在本对象销毁后还回调它。
DisplayViewModel::~DisplayViewModel()
{
    m_worker.stop();
}

// GUI 线程调用：读取并显示所选图片，成功返回 true，失败时报错并返回 false。
bool DisplayViewModel::openImage(const QString &path)
{
    // 图片与摄像头互斥：先停掉采集，保证任何时刻只显示一种来源。
    stopCapture();

    // 用 IMREAD_UNCHANGED 保留原始 alpha 与位深，是否可用交给 labqt 判断。
    const cv::Mat image = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    // 第一重校验：文件打不开，或通道数/位深不受支持。
    if (!labqt::isSupported(image)) {
        emit errorOccurred(QStringLiteral("无法读取图片：%1").arg(path));
        return false;
    }

    // 第二重校验：isSupported 通过也可能转换失败，所以再看 QImage 是否为空。
    const QImage converted = labqt::toQImage(image);
    if (converted.isNull()) {
        emit errorOccurred(QStringLiteral("图片格式不受支持：%1").arg(path));
        return false;
    }

    // 本函数运行在 GUI 线程，信号是直连的，界面可以立刻更新。
    emit frameChanged(converted);
    setMode(Mode::Photo);
    return true;
}

// GUI 线程调用：启动采集线程与刷新定时器并切到视频模式；失败时报错并返回 false。
bool DisplayViewModel::startCamera()
{
    // 先收尾上一次的采集，避免同时存在两条采集线程或一个还在跑的定时器。
    stopCapture();

    // 下面这段回调在采集线程里执行，所以只能碰线程安全的 m_latest，绝不能操作 UI。
    const bool started = m_worker.start([this](labcore::FramePtr frame) {
        // start() 检测到摄像头打不开时会回调 nullptr。
        if (!frame) {
            // 该 emit 发生在采集线程，Qt 会用队列连接把它投递到 GUI 线程的接收者。
            emit errorOccurred(QString::fromStdString(m_worker.lastError()));
            return;
        }
        // 只保留最新一帧：写入即覆盖，界面来不及取走的旧帧被静默丢弃。
        if (labqt::isSupported(frame->mat))
            m_latest.set(labqt::toQImage(frame->mat));
    });
    if (!started) {
        // 线程根本没起来（例如回调为空），同样要报错。
        emit errorOccurred(QString::fromStdString(m_worker.lastError()));
        return false;
    }

    // 注意 started 为 true 只代表线程已启动，摄像头是否打开成功由上面的 nullptr 回调告知。
    m_frameTimer->start();
    setMode(Mode::Video);
    return true;
}

// GUI 线程调用：停止采集并切回空闲模式（界面据此清空画面）。
void DisplayViewModel::stopCamera()
{
    stopCapture();
    setMode(Mode::Idle);
}

// 私有收尾动作：openImage / startCamera / stopCamera 三个入口都先调它。
void DisplayViewModel::stopCapture()
{
    // 先停线程再停定时器：线程停了就不会再有新帧写进单槽。
    m_worker.stop();
    m_frameTimer->stop();
    // 清掉残留帧，否则下次启动时会先闪出上一次的旧画面。
    m_latest.clear();
}

// GUI 线程调用（由 m_frameTimer 超时触发）：取走最新帧再转发给界面。
void DisplayViewModel::pullFrame()
{
    // takeLatest 取走即清空；返回空表示这一拍没有新帧，属于正常的丢帧。
    if (const std::optional<QImage> image = m_latest.takeLatest())
        emit frameChanged(*image);
}

// 只在模式真正改变时发信号，避免界面收到无意义的重复刷新。
void DisplayViewModel::setMode(Mode mode)
{
    if (m_mode == mode)
        return;
    m_mode = mode;
    emit modeChanged(m_mode);
}

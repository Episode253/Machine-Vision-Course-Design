#include "pipeline_viewmodel.h"

#include <chrono>
#include <optional>
#include <utility>

#include <QTimer>

#include "capture_source.h"
#include "qt/logging.h"
#include "qt/qt_interop.h"
#include "register_lab2_stages.h"

PipelineViewModel::PipelineViewModel(QObject *parent)
    : QObject(parent)
{
    registerLab2Stages(m_registry);

    // QTimer 由 QObject 父对象管理；不要再用 unique_ptr 管理同一个 QObject，避免双重释放。
    m_frameTimer = new QTimer(this);
    m_frameTimer->setInterval(33);
    connect(m_frameTimer, &QTimer::timeout, this, &PipelineViewModel::pullFrame);

    m_statsTimer = new QTimer(this);
    m_statsTimer->setInterval(1000);
    connect(m_statsTimer, &QTimer::timeout, this, &PipelineViewModel::refreshStats);
}

// 析构：停止管线并释放源；两个 QTimer 以 this 为父对象，由 Qt 负责删除。
PipelineViewModel::~PipelineViewModel()
{
    // 顺序不能反：管线只借用 m_source，必须先 join 掉工作线程再销毁源，
    // 否则工作线程可能在源析构之后继续调用 next。
    stopPipeline();
    releaseSource();
}

bool PipelineViewModel::running() const
{
    return m_pipeline && m_pipeline->running();
}

// 打开摄像头开始预览；失败时释放源并回到空闲态，界面据此提示摄像头不可用。
// 先清空 m_currentImagePath，使源目标从图片变为摄像头，从而触发源的重新创建。
bool PipelineViewModel::startCamera()
{
    m_currentImagePath.clear();
    if (!startInternal(QString())) {
        releaseSource();
        setMode(Mode::Idle);
        return false;
    }
    setMode(Mode::Video);
    return true;
}

// 打开图片文件开始预览；路径为空直接失败，失败同样释放源并回到空闲态。
bool PipelineViewModel::startImage(const QString &path)
{
    if (path.isEmpty())
        return false;
    m_currentImagePath = path;
    if (!startInternal(path)) {
        releaseSource();
        setMode(Mode::Idle);
        return false;
    }
    setMode(Mode::Photo);
    return true;
}

// 用户主动停止：连源一起释放，下次启动会重新打开设备。
void PipelineViewModel::stop()
{
    stopPipeline();

    releaseSource();
    setMode(Mode::Idle);
}

// 只停管线不释放源，供切换 Sobel 开关时复用同一路源。
void PipelineViewModel::stopPipeline()
{
    if (m_frameTimer)
        m_frameTimer->stop();
    if (m_statsTimer)
        m_statsTimer->stop();
    if (m_pipeline) {
        m_pipeline->stop();
        // 停管线会 join 所有工作线程；随后 reset 释放旧管线及其 stage 实例，
        // 因为下一次 startInternal 要重新装配阶段列表，而运行中的管线不接受增删阶段。
        m_pipeline.reset();
    }
    // 丢掉槽里可能残留的旧帧，避免停止之后界面又刷出一帧。
    m_latest.clear();
}

// 关闭并销毁源，同时清掉来源标记，使下次 startInternal 必定重建源。
void PipelineViewModel::releaseSource()
{
    if (m_source) {
        m_source->close();
        m_source.reset();
    }
    m_sourceOpen = false;
    m_sourceTarget.clear();
}

// Sobel 开关：阶段列表变了，只能在未运行状态下重建管线，因此这里重启一次；
// 未运行时只记录标志，等下次启动生效。
void PipelineViewModel::setSobelEnabled(bool enabled)
{
    // 值没变就直接返回：重复勾选同一个状态会白重启一次管线。
    if (m_sobelEnabled == enabled)
        return;
    m_sobelEnabled = enabled;
    qCInfo(labPipeline) << "sobel stage" << (enabled ? "enabled" : "disabled");
    if (running() && !startInternal(m_currentImagePath)) {
        releaseSource();
        setMode(Mode::Idle);
    }
}

// 组装管线：按需重建源、装配阶段、设置输出回调，最后 build 加 start。
bool PipelineViewModel::startInternal(const QString &imagePath)
{
    stopPipeline();
    // 源只在没有源、未打开、目标变化这三种情况下重建。切换 Sobel 开关走的也是
    // 这个函数，此时路径与 m_sourceTarget 相同，已打开的摄像头会被直接复用，
    // 不会因重新 open 而中断采集。
    // 另外管线只借用源，打开动作必须在这里显式完成。
    if (!m_source || !m_sourceOpen || m_sourceTarget != imagePath) {
        m_source = std::make_unique<CaptureSource>(imagePath);
        std::string error;
        if (!m_source->open(&error)) {
            m_sourceOpen = false;
            emit errorOccurred(QString::fromStdString(
                error.empty() ? "capture source could not be opened" : error));
            return false;
        }
        m_sourceTarget = imagePath;
        m_sourceOpen = true;
    }

    m_pipeline = std::make_unique<labruntime::Pipeline>();

    // Pipeline 此处仅借用 Source；ViewModel 保持 m_source 的所有权，
    // 因而停止管线后仍可复用已打开的摄像头或图片源。
    m_pipeline->setSource(m_source.get());
    // Sobel 关闭时直接显示原始彩色帧（与手册实验二的默认效果一致）；
    // 只有开启 Sobel 时才先转灰度、再求边缘。
    if (m_sobelEnabled) {
        m_pipeline->addStage(m_registry.create("gray"));
        m_pipeline->addStage(m_registry.create("sobel"));
    }
    m_pipeline->setOutput([this](labcore::FramePtr frame) {
        // 该回调跑在管线的工作线程上，所以只写线程安全的单槽，不碰任何 QWidget；
        // GUI 线程由 m_frameTimer 定时取走。
        // toQImage 内部会 copy 一份，离开这个作用域后不再引用 cv::Mat 的缓冲。
        m_latest.set(labqt::toQImage(frame->mat));
    });

    // build 会校验节点名与格式链（Sobel 之前必须是 Gray8），start 才真正起线程；
    // 两者都要检查，失败时把 lastError 交给界面。
    if (!m_pipeline->build() || !m_pipeline->start()) {
        emit errorOccurred(QString::fromStdString(m_pipeline->lastError()));
        return false;
    }
    m_frameTimer->start();
    m_statsTimer->start();
    return true;
}

// 模式变化才发信号，避免界面重复刷新按钮文案与画面。
void PipelineViewModel::setMode(Mode mode)
{
    if (m_mode == mode)
        return;
    m_mode = mode;
    emit modeChanged(m_mode);
}

// GUI 线程的 33ms 定时器：取走槽里的最新一帧转发给界面，没有新帧时什么都不做。
void PipelineViewModel::pullFrame()
{
    if (const std::optional<QImage> image = m_latest.takeLatest())
        emit frameChanged(*image);
}

// GUI 线程的 1s 定时器：向状态栏报 fps 与丢帧数，并发现采集线程的意外退出。
void PipelineViewModel::refreshStats()
{
    if (!m_pipeline)
        return;
    const labruntime::Pipeline::Stats stats = m_pipeline->stats();
    emit statsChanged(stats.fps, stats.dropped);
    if (!stats.running) {
        // stats.running 由真变假，说明采集线程已自行退出（摄像头出错或取帧失败），
        // 此时主动收尾并回到空闲态。
        stopPipeline();
        releaseSource();
        setMode(Mode::Idle);
    }
}

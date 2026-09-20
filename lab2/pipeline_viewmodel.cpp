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

PipelineViewModel::~PipelineViewModel()
{



    stopPipeline();
    releaseSource();
}

bool PipelineViewModel::running() const
{
    return m_pipeline && m_pipeline->running();
}

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

void PipelineViewModel::stop()
{
    stopPipeline();

    releaseSource();
    setMode(Mode::Idle);
}

void PipelineViewModel::stopPipeline()
{
    if (m_frameTimer)
        m_frameTimer->stop();
    if (m_statsTimer)
        m_statsTimer->stop();
    if (m_pipeline) {
        m_pipeline->stop();


        m_pipeline.reset();
    }


    m_latest.clear();
}

void PipelineViewModel::releaseSource()
{
    if (m_source) {
        m_source->close();
        m_source.reset();
    }
    m_sourceOpen = false;
    m_sourceTarget.clear();
}

void PipelineViewModel::setSobelEnabled(bool enabled)
{
    if (m_sobelEnabled == enabled)
        return;
    m_sobelEnabled = enabled;
    qCInfo(labPipeline) << "sobel stage" << (enabled ? "enabled" : "disabled");
    if (running() && !startInternal(m_currentImagePath)) {
        releaseSource();
        setMode(Mode::Idle);
    }
}

bool PipelineViewModel::startInternal(const QString &imagePath)
{
    stopPipeline();




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
    // TODO(lab2): 勾选 Sobel 时，按顺序装配“灰度 + 边缘”两个 stage。
    // TODO(lab2): 注意顺序不能反（否则管线格式链校验失败）。
    m_pipeline->setOutput([this](labcore::FramePtr frame) {

        m_latest.set(labqt::toQImage(frame->mat));
    });

    if (!m_pipeline->build() || !m_pipeline->start()) {
        emit errorOccurred(QString::fromStdString(m_pipeline->lastError()));
        return false;
    }
    m_frameTimer->start();
    m_statsTimer->start();
    return true;
}

void PipelineViewModel::setMode(Mode mode)
{
    if (m_mode == mode)
        return;
    m_mode = mode;
    emit modeChanged(m_mode);
}

void PipelineViewModel::pullFrame()
{
    if (const std::optional<QImage> image = m_latest.takeLatest())
        emit frameChanged(*image);
}

void PipelineViewModel::refreshStats()
{
    if (!m_pipeline)
        return;
    const labruntime::Pipeline::Stats stats = m_pipeline->stats();
    emit statsChanged(stats.fps, stats.dropped);
    if (!stats.running) {


        stopPipeline();
        releaseSource();
        setMode(Mode::Idle);
    }
}

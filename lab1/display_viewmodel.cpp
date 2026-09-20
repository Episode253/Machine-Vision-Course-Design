#include "display_viewmodel.h"

#include <chrono>
#include <optional>

#include <opencv2/imgcodecs.hpp>

#include <QTimer>

#include "qt/logging.h"
#include "qt/qt_interop.h"

DisplayViewModel::DisplayViewModel(QObject *parent)
    : QObject(parent)
{
    m_frameTimer = new QTimer(this);
    m_frameTimer->setInterval(33);
    connect(m_frameTimer, &QTimer::timeout, this, &DisplayViewModel::pullFrame);
}

DisplayViewModel::~DisplayViewModel()
{
    m_worker.stop();
}

bool DisplayViewModel::openImage(const QString &path)
{
    stopCapture();

    const cv::Mat image = cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    if (!labqt::isSupported(image)) {
        emit errorOccurred(QStringLiteral("无法读取图片：%1").arg(path));
        return false;
    }

    const QImage converted = labqt::toQImage(image);
    if (converted.isNull()) {
        emit errorOccurred(QStringLiteral("图片格式不受支持：%1").arg(path));
        return false;
    }

    emit frameChanged(converted);
    setMode(Mode::Photo);
    return true;
}

bool DisplayViewModel::startCamera()
{
    stopCapture();

    const bool started = m_worker.start([this](labcore::FramePtr frame) {
        if (!frame) {
            emit errorOccurred(QString::fromStdString(m_worker.lastError()));
            return;
        }
        if (labqt::isSupported(frame->mat))
            m_latest.set(labqt::toQImage(frame->mat));
    });
    if (!started) {
        emit errorOccurred(QString::fromStdString(m_worker.lastError()));
        return false;
    }

    m_frameTimer->start();
    setMode(Mode::Video);
    return true;
}

void DisplayViewModel::stopCamera()
{
    stopCapture();
    setMode(Mode::Idle);
}

void DisplayViewModel::stopCapture()
{
    m_worker.stop();
    m_frameTimer->stop();
    m_latest.clear();
}

void DisplayViewModel::pullFrame()
{
    if (const std::optional<QImage> image = m_latest.takeLatest())
        emit frameChanged(*image);
}

void DisplayViewModel::setMode(Mode mode)
{
    if (m_mode == mode)
        return;
    m_mode = mode;
    emit modeChanged(m_mode);
}

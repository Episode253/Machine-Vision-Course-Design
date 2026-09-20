#include "frame_source.h"

#include "qt/logging.h"

bool FrameSource::open(int cameraIndex)
{
    m_cameraIndex = cameraIndex;

    const QString imagePath = QString::fromLocal8Bit(qgetenv("VISUAL_LAB_FRAME_IMAGE"));
    if (!imagePath.isEmpty())
        return openImage(imagePath);

    release();

    try { m_camera.open(m_cameraIndex, cv::CAP_ANY); }
    catch (const cv::Exception &error) { m_lastError = QString::fromLocal8Bit(error.what()); }
    if (!m_camera.isOpened() && m_lastError.isEmpty())
        m_lastError = QStringLiteral("camera %1 is unavailable").arg(m_cameraIndex);
    return m_camera.isOpened();
}

bool FrameSource::openImage(const QString &imagePath)
{
    release();
    if (imagePath.isEmpty()) {
        m_lastError = QStringLiteral("empty image path");
        return false;
    }
    m_testFrame = cv::imread(imagePath.toStdString(), cv::IMREAD_UNCHANGED);
    if (m_testFrame.empty()) {
        m_lastError = QStringLiteral("cannot load image: %1").arg(imagePath);
        qCWarning(labCamera) << m_lastError;
        return false;
    }
    m_usingTestFrame = true;
    m_testImagePath = imagePath;
    qCInfo(labCamera) << "using repeated image" << imagePath;
    return true;
}

std::optional<labcore::Frame> FrameSource::next()
{
    if (m_usingTestFrame) {
        if (m_testFrame.empty())
            return std::nullopt;
        return labcore::makeFrame(m_testFrame.clone(), ++m_seq);
    }
    if (!m_camera.isOpened())
        return std::nullopt;
    cv::Mat frame;
    if (!m_camera.read(frame) || frame.empty()) {
        m_lastError = QStringLiteral("camera %1 returned an empty frame").arg(m_cameraIndex);
        return std::nullopt;
    }
    return labcore::makeFrame(std::move(frame), ++m_seq);
}

bool FrameSource::isOpened() const
{
    return m_usingTestFrame || m_camera.isOpened();
}

void FrameSource::release()
{
    if (m_camera.isOpened()) m_camera.release();
    m_testFrame.release();
    m_usingTestFrame = false;
    m_seq = 0;
    m_testImagePath.clear();
    m_lastError.clear();
}

QString FrameSource::description() const
{
    if (m_usingTestFrame)
        return m_testImagePath.isEmpty() ? QStringLiteral("test image") : m_testImagePath;
    return QStringLiteral("camera %1").arg(m_cameraIndex);
}

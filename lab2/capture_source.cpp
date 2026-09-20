#include "capture_source.h"

#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#include "qt/logging.h"

CaptureSource::CaptureSource(QString imagePath)
    : m_imagePath(std::move(imagePath))
{
}

std::string_view CaptureSource::name() const
{
    return m_imagePath.isEmpty() ? std::string_view("camera") : std::string_view("image");
}

bool CaptureSource::open(std::string *error)
{
    m_stopped.store(false);
    const bool opened = m_imagePath.isEmpty() ? m_source.open() : m_source.openImage(m_imagePath);
    if (opened) {
        qCInfo(labPipeline) << "source" << name().data() << "opened:" << m_source.description();
        return true;
    }
    if (error)
        *error = m_source.lastError().toStdString();
    return false;
}

void CaptureSource::close()
{
    m_stopped.store(true);
    m_source.release();
}

labcore::FramePtr CaptureSource::next()
{
    if (m_stopped.load())
        return nullptr;

    const std::optional<labcore::Frame> frame = m_source.next();
    if (!frame || frame->mat.empty())
        return nullptr;

    if (!m_imagePath.isEmpty())
        std::this_thread::sleep_for(std::chrono::milliseconds(33));

    return std::make_shared<const labcore::Frame>(std::move(*frame));
}

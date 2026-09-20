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
    // TODO(lab2): 已请求停止或源取不到帧时返回空。
    // TODO(lab2): 测试图片模式下稍作等待以模拟帧率。
    // TODO(lab2): 把取到的帧打包成共享帧返回。
    return nullptr;
}

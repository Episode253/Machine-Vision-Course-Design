#include "capture_source.h"

#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#include "qt/logging.h"

// 以图片路径构造源对象；路径为空表示用摄像头，非空表示用图片。
CaptureSource::CaptureSource(QString imagePath)
    : m_imagePath(std::move(imagePath))
{
}

// 源节点名：供管线日志与 build 阶段的重名校验使用；空路径对应 "camera"，否则 "image"。
std::string_view CaptureSource::name() const
{
    return m_imagePath.isEmpty() ? std::string_view("camera") : std::string_view("image");
}

// 打开摄像头或图片；由 GUI 线程在启动管线之前调用（管线只借用本对象，不负责打开）。
bool CaptureSource::open(std::string *error)
{
    // 复位停止标志：close 会把它置位，同一个源对象停止之后还要能再次开放。
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

// 请求停止并释放底层设备；m_stopped 是跨线程标志，
// 工作线程的 next 见到它会立即返回空，从而结束采集循环。
void CaptureSource::close()
{
    m_stopped.store(true);
    m_source.release();
}

// 取一帧并打包成共享帧；由管线的工作线程循环调用，返回空即表示源已结束。
labcore::FramePtr CaptureSource::next()
{
    // 先看停止标志：close 之后不必再去碰底层设备。
    if (m_stopped.load())
        return nullptr;

    const std::optional<labcore::Frame> frame = m_source.next();
    // 摄像头出错或读到空帧时同样返回空，让管线自然退出采集循环而不是空转。
    if (!frame || frame->mat.empty())
        return nullptr;
    // 图片是静止的，取帧不阻塞，这里补一次 33ms 等待以模拟摄像头帧率；
    // 否则会以 CPU 全速出帧，fps 与丢帧统计也就失去意义。
    if (!m_imagePath.isEmpty())
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    // move 进 const 共享帧：发布后不可变、零拷贝传给下游；格式由 makeFrame 按通道数推断。
    return std::make_shared<const labcore::Frame>(std::move(*frame));
}

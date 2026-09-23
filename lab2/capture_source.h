#ifndef LAB2_CAPTURE_SOURCE_H
#define LAB2_CAPTURE_SOURCE_H

#include <atomic>
#include <cstdint>

#include <QString>

#include "core/stage.hpp"
#include "runtime/frame_source.h"



// lab2 的管线源阶段：把内部 FrameSource 适配成 labcore::Source 接口。
// 摄像头与图片二选一由构造时路径是否为空决定，outputFormat 恒为 BGR8。
class CaptureSource : public labcore::Source {
public:
    CaptureSource() = default;
    explicit CaptureSource(QString imagePath);

    std::string_view name() const override;
    labcore::PixelFormat outputFormat() const override { return labcore::PixelFormat::BGR8; }

    bool open(std::string *error) override;
    void close() override;



    // 阶段接口的取帧函数；返回空表示源结束或出错，管线据此终止采集循环。
    // 由管线的工作线程调用，close 可能在另一线程并发置位 m_stopped。
    labcore::FramePtr next() override;

private:
    QString m_imagePath;
    FrameSource m_source;
    std::atomic_bool m_stopped{false};
};

#endif

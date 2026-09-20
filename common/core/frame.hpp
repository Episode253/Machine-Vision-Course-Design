#ifndef VISUAL_LAB_CORE_FRAME_H
#define VISUAL_LAB_CORE_FRAME_H

#include <chrono>
#include <cstdint>
#include <memory>
#include <utility>

#include <opencv2/core.hpp>

namespace labcore {



enum class PixelFormat { BGR8, BGRA8, Gray8 };

inline int channelCount(PixelFormat format)
{
    switch (format) {
    case PixelFormat::BGR8:
        return 3;
    case PixelFormat::BGRA8:
        return 4;
    case PixelFormat::Gray8:
        return 1;
    }
    return 3;
}

inline PixelFormat formatOf(const cv::Mat &mat)
{
    switch (mat.channels()) {
    case 1:
        return PixelFormat::Gray8;
    case 4:
        return PixelFormat::BGRA8;
    default:
        return PixelFormat::BGR8;
    }
}



struct Frame {
    PixelFormat format = PixelFormat::BGR8;
    cv::Mat mat;
    std::uint64_t seq = 0;
    std::chrono::steady_clock::time_point timestamp{};
};

inline Frame makeFrame(cv::Mat mat, std::uint64_t seq)
{
    Frame frame;
    frame.format = formatOf(mat);
    frame.mat = std::move(mat);
    frame.seq = seq;
    frame.timestamp = std::chrono::steady_clock::now();
    return frame;
}

// 帧在多个处理阶段间共享且只读；shared_ptr<const Frame> 明确共享所有权，
// 不可变约束避免某个阶段原地修改数据而影响其他消费者。
using FramePtr = std::shared_ptr<const Frame>;

}

#endif

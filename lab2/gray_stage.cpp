#include "gray_stage.h"

#include <memory>
#include <utility>

#include <opencv2/imgproc.hpp>

// 灰度转换：BGR8 输入、Gray8 输出；本阶段为 Inline 执行，
// 不单独开线程，由管线在源线程取帧之后同步调用。
labcore::FramePtr GrayStage::process(const labcore::FramePtr &input)
{
    // 上游异常时返回空，管线按丢帧处理并结束这一条链路。
    if (!input || input->mat.empty())
        return nullptr;
    cv::Mat gray;
    cv::cvtColor(input->mat, gray, cv::COLOR_BGR2GRAY);
    // 保留输入的 seq 作为帧标识，时间戳由 makeFrame 重新盖章。
    return std::make_shared<labcore::Frame>(labcore::makeFrame(std::move(gray), input->seq));
}

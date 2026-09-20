#include "gray_stage.h"

#include <memory>
#include <utility>

#include <opencv2/imgproc.hpp>

labcore::FramePtr GrayStage::process(const labcore::FramePtr &input)
{
    if (!input || input->mat.empty())
        return nullptr;
    cv::Mat gray;
    cv::cvtColor(input->mat, gray, cv::COLOR_BGR2GRAY);
    return std::make_shared<labcore::Frame>(labcore::makeFrame(std::move(gray), input->seq));
}

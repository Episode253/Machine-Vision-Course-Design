#include "sobel_stage.h"

#include <memory>
#include <utility>

#include <opencv2/imgproc.hpp>

SobelStage::SobelStage()
    : m_params(std::make_shared<const SobelParams>())
{
}

labcore::FramePtr SobelStage::process(const labcore::FramePtr &input)
{
    if (!input || input->mat.empty())
        return nullptr;

    std::shared_ptr<const SobelParams> params;
    {
        std::lock_guard<std::mutex> lock(m_paramsMutex);
        params = m_params;
    }

    cv::Mat edges;
    cv::Sobel(input->mat, edges, CV_16S, params->dx, params->dy, params->ksize);
    cv::convertScaleAbs(edges, edges);
    return std::make_shared<const labcore::Frame>(labcore::makeFrame(std::move(edges), input->seq));
}

void SobelStage::setParams(std::shared_ptr<const labcore::StageParams> params)
{
    if (auto typed = std::dynamic_pointer_cast<const SobelParams>(std::move(params))) {
        std::lock_guard<std::mutex> lock(m_paramsMutex);
        m_params = std::move(typed);
    }
}

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
    // TODO(lab2): 校验输入帧有效，无效则返回空。
    // TODO(lab2): 按当前参数做 Sobel 边缘检测，返回处理后的新帧。
    (void)input;
    return nullptr;
}

void SobelStage::setParams(std::shared_ptr<const labcore::StageParams> params)
{
    if (auto typed = std::dynamic_pointer_cast<const SobelParams>(std::move(params))) {
        std::lock_guard<std::mutex> lock(m_paramsMutex);
        m_params = std::move(typed);
    }
}

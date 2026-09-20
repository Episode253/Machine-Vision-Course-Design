#ifndef LAB2_SOBEL_STAGE_H
#define LAB2_SOBEL_STAGE_H

#include <memory>
#include <mutex>
#include <string_view>

#include "core/stage.hpp"



struct SobelParams : labcore::StageParams {
    int dx = 1;
    int dy = 0;
    int ksize = 3;
};

class SobelStage : public labcore::Stage {
public:
    SobelStage();

    std::string_view name() const override { return "sobel"; }
    labcore::PixelFormat inputFormat() const override { return labcore::PixelFormat::Gray8; }
    labcore::PixelFormat outputFormat() const override { return labcore::PixelFormat::Gray8; }



    labcore::FramePtr process(const labcore::FramePtr &input) override;



    void setParams(std::shared_ptr<const labcore::StageParams> params) override;

private:
    std::shared_ptr<const SobelParams> m_params;
    mutable std::mutex m_paramsMutex;
};

#endif

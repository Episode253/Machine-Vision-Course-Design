#ifndef LAB2_GRAY_STAGE_H
#define LAB2_GRAY_STAGE_H

#include <string_view>

#include "core/stage.hpp"



class GrayStage : public labcore::Stage {
public:
    std::string_view name() const override { return "gray"; }
    labcore::PixelFormat inputFormat() const override { return labcore::PixelFormat::BGR8; }
    labcore::PixelFormat outputFormat() const override { return labcore::PixelFormat::Gray8; }
    Execution execution() const override { return Execution::Inline; }

    labcore::FramePtr process(const labcore::FramePtr &input) override;
};

#endif

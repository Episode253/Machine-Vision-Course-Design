#ifndef LAB2_GRAY_STAGE_H
#define LAB2_GRAY_STAGE_H

#include <string_view>

#include "core/stage.hpp"



// 灰度阶段：把 BGR8 输入转成 Gray8 输出。
// 它是 Sobel 的前置步骤——Sobel 阶段声明只接受 Gray8。
class GrayStage : public labcore::Stage {
public:
    std::string_view name() const override { return "gray"; }
    // 声明的输入/输出格式是管线 build 时格式链校验的依据，
    // 链上每一环的 inputFormat 必须等于上一环的 outputFormat。
    labcore::PixelFormat inputFormat() const override { return labcore::PixelFormat::BGR8; }
    labcore::PixelFormat outputFormat() const override { return labcore::PixelFormat::Gray8; }
    // 转换成本极低，不值得单开线程和一级队列，直接在源线程里做。
    Execution execution() const override { return Execution::Inline; }

    labcore::FramePtr process(const labcore::FramePtr &input) override;
};

#endif

#include "register_lab2_stages.h"

#include <memory>

#include "gray_stage.h"
#include "sobel_stage.h"

void registerLab2Stages(labcore::StageRegistry &registry)
{
    registry.registerStage("gray", [] { return std::make_unique<GrayStage>(); });
    registry.registerStage("sobel", [] { return std::make_unique<SobelStage>(); });
}

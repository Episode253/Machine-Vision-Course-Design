#include "register_lab2_stages.h"

#include <memory>

#include "gray_stage.h"
#include "sobel_stage.h"

// 把 lab2 的两个阶段以 "gray"、"sobel" 为名注册进注册表；
// 由 PipelineViewModel 的构造函数在 GUI 线程调用一次。
void registerLab2Stages(labcore::StageRegistry &registry)
{
    // 每个名字对应一个工厂 lambda：每次 create 都新建实例并交出 unique_ptr。
    registry.registerStage("gray", [] { return std::make_unique<GrayStage>(); });
    registry.registerStage("sobel", [] { return std::make_unique<SobelStage>(); });
}

#ifndef LAB2_REGISTER_LAB2_STAGES_H
#define LAB2_REGISTER_LAB2_STAGES_H

#include "core/stage_registry.h"



// 向注册表登记 lab2 用到的阶段名与工厂；Pipeline 只管按名字取用，
// ViewModel 因此不必依赖 GrayStage / SobelStage 的具体类型。
// 同名重复注册会失败，所以构造时调用一次即可。
void registerLab2Stages(labcore::StageRegistry &registry);

#endif

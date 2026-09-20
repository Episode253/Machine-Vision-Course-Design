#ifndef VISUAL_LAB_CORE_STAGE_REGISTRY_H
#define VISUAL_LAB_CORE_STAGE_REGISTRY_H

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "stage.hpp"

namespace labcore {




class StageRegistry {
public:
    // 工厂返回 unique_ptr，阶段的唯一所有权随后转交给 Pipeline；
    // unique_ptr 本身就是 RAII 封装，不需要再为同一对象维护裸指针所有者。
    using Factory = std::function<std::unique_ptr<Stage>()>;

    bool registerStage(std::string name, Factory factory);
    std::unique_ptr<Stage> create(std::string_view name) const;
    bool contains(std::string_view name) const;
    std::vector<std::string> names() const;

private:
    std::map<std::string, Factory, std::less<>> m_factories;
};

}

#endif

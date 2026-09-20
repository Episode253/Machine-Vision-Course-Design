#include "stage_registry.h"

namespace labcore {

bool StageRegistry::registerStage(std::string name, Factory factory)
{
    if (name.empty() || !factory)
        return false;
    return m_factories.emplace(std::move(name), std::move(factory)).second;
}

std::unique_ptr<Stage> StageRegistry::create(std::string_view name) const
{
    const auto it = m_factories.find(name);
    if (it == m_factories.end())
        return nullptr;
    return it->second();
}

bool StageRegistry::contains(std::string_view name) const
{
    return m_factories.find(name) != m_factories.end();
}

std::vector<std::string> StageRegistry::names() const
{
    std::vector<std::string> result;
    result.reserve(m_factories.size());
    for (const auto &entry : m_factories)
        result.push_back(entry.first);
    return result;
}

}

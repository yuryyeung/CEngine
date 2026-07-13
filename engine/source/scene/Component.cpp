#include "scene/Component.h"
#include <cstddef>
#include <nlohmann/json.hpp>

namespace CEngine
{
    size_t Component::nextId = 1;

    void Component::Init()
    {
        
    }

    GameObject *Component::GetOwner()
    {
        return m_owner;
    }

    ComponentFactory& ComponentFactory::GetInstance()
    {
        static ComponentFactory instance;
        return instance;
    }

    void Component::LoadProperties(const nlohmann::json& json)
    {

    }
}
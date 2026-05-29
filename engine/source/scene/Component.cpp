#include "scene/Component.h"
#include <cstddef>

namespace CEngine
{
    size_t Component::nextId = 1;

    GameObject *Component::GetOwner()
    {
        return m_owner;
    }
}
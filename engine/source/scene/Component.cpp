#include "scene/Component.h"

namespace CEngine
{
    GameObject *Component::GetOwner()
    {
        return m_owner;
    }
}
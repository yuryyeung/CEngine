#include "Application.h"

namespace CEngine
{
    void Application::SetNeedToBeClosed(bool value)
    {
        m_needsToBeClosed = value;
    }

    bool Application::NeedToBeClosed() const
    {
        return m_needsToBeClosed;
    }
}
#include "input/InputManager.h"
#include "utils/Debug.h"

namespace CEngine
{
    void InputManager::SetKeyPressed(int key, bool pressed)
    {
        if (key < 0 || key >= static_cast<int>(m_keys.size()))
        {
            return;
        }
        m_keys[key] = pressed;
    }

    bool InputManager::IsKeyPressed(int key)
    {
        if (key < 0 || key >= static_cast<int>(m_keys.size()))
        {
            return false;
        }

        return m_keys[key];
    }

    void InputManager::SetMouseButtonPressed(int button, bool pressed)
    {
        if (button < 0 || button >= static_cast<int>(m_mouseKeys.size()))
        {
            return;
        }
        m_mouseKeys[button] = pressed;
    }

    bool InputManager::IsMouseButtonPressed(int button)
    {
        if (button < 0 || button >= static_cast<int>(m_mouseKeys.size()))
        {
            return false;
        }

        return m_mouseKeys[button];
    }

    void InputManager::SetMousePositionOld(const glm::vec2 &position)
    {
        Debug::Log("Update Previous [" + std::to_string(position.x) + ", " + std::to_string(position.y) + "]");
        m_mousePositionOld = position;
    }

    const glm::vec2 &InputManager::GetMousePositionOld() const
    {
        return m_mousePositionOld;
    }

    void InputManager::SetMousePositionCurrent(const glm::vec2 &position)
    {
        glm::vec2 changes = position - this->GetMousePositionOld();
        Debug::Log("Update Current [" + std::to_string(position.x) + ", " + std::to_string(position.y) + "]");
        m_mousePositionCurrent = position;
    }

    void InputManager::MouseTracing(double xpos, double ypos)
    {
        glm::vec2 currentPos(static_cast<float>(xpos), static_cast<float>(ypos));
        // Debug::Log(std::to_string(xpos) + " " + std::to_string(ypos));
        if (m_isStarted)
        {
            auto previousPos = GetMousePositionCurrent();
            if (GetMousePositionCurrent() != currentPos)
            {
                SetMousePositionChanged(true);
            }
            SetMousePositionOld(GetMousePositionCurrent());
        }
        else if (GetMousePositionCurrent() != glm::vec2(0.0f))
        {
            m_isStarted = true;
        }
        SetMousePositionCurrent(currentPos);

        glm::vec2 old = GetMousePositionOld();
        // Debug::Log("old:[x: " + std::to_string(old.x) + ",y: " + std::to_string(old.y) + "] | current[x: " + std::to_string(currentPos.x) + ",y: " + std::to_string(currentPos.y) + "]");
    }

    bool InputManager::GetIsStarted()
    {
        return m_isStarted;
    }

    const glm::vec2 &InputManager::GetMousePositionCurrent() const
    {
        return m_mousePositionCurrent;
    }

    void InputManager::SetMousePositionChanged(bool changed)
    {
        m_mousePositionChanged = changed;
    }

    bool InputManager::IsMousePositionChanged()
    {
        return m_mousePositionChanged;
    }
}
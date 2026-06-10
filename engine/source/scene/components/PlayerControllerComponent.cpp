#include "scene/components/PlayerControllerComponent.h"
#include "input/InputManager.h"
#include "Engine.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>

namespace CEngine
{
    void PlayerControllerComponent::Update(float deltaTime)
    {
        auto &inputManager = Engine::GetInstance().GetInputManager();
        auto rotation = m_owner->GetRotation();

        if (inputManager.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            const auto &oldPos = inputManager.GetMousePositionOld();
            const auto &current = inputManager.GetMousePositionCurrent();

            float deltaX = current.x - oldPos.x;
            float deltaY = current.y - oldPos.y;

            // Rotation Around Y Axis
            float yAngle = -deltaX * m_sensitivity * deltaTime;
            glm::quat yRot = glm::angleAxis(yAngle, glm::vec3(0.0f, 1.0f, 0.0f));

            // Rotation Around X Axis
            float xAngle = -deltaY * m_sensitivity * deltaTime;
            glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
            glm::quat xRot = glm::angleAxis(xAngle, right);

            glm::quat deltaRot = yRot * xRot;
            rotation = glm::normalize(deltaRot * rotation);

            m_owner->SetRotation(rotation);
        }

        // glm::mat4 rotMax(1.0f);
        // rotMax = glm::rotate(rotMax, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
        // rotMax = glm::rotate(rotMax, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis
        // rotMax = glm::rotate(rotMax, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis

        // glm::vec3 front = glm::normalize(glm::vec3(rotMax * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
        // glm::vec3 right = glm::normalize(glm::vec3(rotMax * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));

        glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);

        auto position = m_owner->GetPosition();

        // Horizotal Movement
        if (inputManager.IsKeyPressed(GLFW_KEY_A))
        {
            position -= right * m_moveSpeed * deltaTime;
        }
        if (inputManager.IsKeyPressed(GLFW_KEY_D))
        {
            position += right * m_moveSpeed * deltaTime;
        }

        // Vertical Movement
        if (inputManager.IsKeyPressed(GLFW_KEY_W))
        {
            position += front * m_moveSpeed * deltaTime;
        }
        if (inputManager.IsKeyPressed(GLFW_KEY_S))
        {
            position -= front * m_moveSpeed * deltaTime;
        }
        m_owner->SetPosition(position);
    }
}
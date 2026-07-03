#include "scene/components/PlayerControllerComponent.h"
#include "input/InputManager.h"
#include "Engine.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>
#include "utils/Debug.h"
#include <cmath>
#include <numbers>

namespace CEngine
{
    void PlayerControllerComponent::Init()
    {
        m_kinematicController = std::make_unique<KinematicCharacterController>(0.4f, 1.2f);
    }

    void PlayerControllerComponent::Update(float deltaTime)
    {
        auto &inputManager = Engine::GetInstance().GetInputManager();
        auto rotation = m_owner->GetRotation();

        if (inputManager.IsMousePositionChanged())
        {
            const auto &oldPos = inputManager.GetMousePositionOld();
            const auto &current = inputManager.GetMousePositionCurrent();

            float deltaX = current.x - oldPos.x;
            float deltaY = current.y - oldPos.y;

            // // Rotation Around Y Axis
            // float yDeltaAngle = -deltaX * m_sensitivity ;
            // m_yRot += yDeltaAngle;
            // glm::quat yRot = glm::angleAxis(glm::radians(m_yRot), glm::vec3(0.0f, 1.0f, 0.0f));

            // // Rotation Around X Axis
            // float xDeltaAngle = -deltaY * m_sensitivity;
            // m_xRot += xDeltaAngle;
            // m_xRot = glm::clamp(m_xRot, -89.0f, 89.0f);
            // glm::quat xRot = glm::angleAxis(glm::radians(m_xRot), glm::vec3(1.0f, 0.0f, 0.0f));

            // rotation = glm::normalize(yRot * xRot);

            m_yRot -= deltaX * m_sensitivity * deltaTime; // (Yaw)
            m_xRot -= deltaY * m_sensitivity * deltaTime; // (Pitch)

            double pi = M_PI;

            m_xRot = glm::clamp(m_xRot, -85.0f, 85.0f);

            glm::quat yaw = CreateQuaternion(glm::vec3(0.0f, 1.0f, 0.0f), static_cast<float>(m_yRot * (pi / 180.0f)));
            glm::quat pitch = CreateQuaternion(glm::vec3(1.0f, 0.0f, 0.0f), static_cast<float>(m_xRot * (pi / 180.0f)));
            rotation = yaw * pitch;

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

        // auto position = m_owner->GetPosition();

        // // Horizotal Movement
        // if (inputManager.IsKeyPressed(GLFW_KEY_A))
        // {
        //     position -= right * m_moveSpeed * deltaTime;
        // }
        // if (inputManager.IsKeyPressed(GLFW_KEY_D))
        // {
        //     position += right * m_moveSpeed * deltaTime;
        // }

        // // Vertical Movement
        // if (inputManager.IsKeyPressed(GLFW_KEY_W))
        // {
        //     position += front * m_moveSpeed * deltaTime;
        // }
        // if (inputManager.IsKeyPressed(GLFW_KEY_S))
        // {
        //     position -= front * m_moveSpeed * deltaTime;
        // }
        // m_owner->SetPosition(position);

        glm::vec3 move(0.0f);
        // Horizotal Movement
        if (inputManager.IsKeyPressed(GLFW_KEY_A))
        {
            move -= right;
        }
        if (inputManager.IsKeyPressed(GLFW_KEY_D))
        {
            move += right;
        }

        // Vertical Movement
        if (inputManager.IsKeyPressed(GLFW_KEY_W))
        {
            move += front;
        }
        if (inputManager.IsKeyPressed(GLFW_KEY_S))
        {
            move -= front;
        }

        // Jump
        if (inputManager.IsKeyPressed(GLFW_KEY_SPACE))
        {
            m_kinematicController->Jump(glm::vec3(0.0f, 5.0f, 0.0f));
        }

        if (glm::dot(move, move) > 0)
        {
            move = glm::normalize(move);
        }
        m_kinematicController->Walk(move * m_moveSpeed * deltaTime);
        m_owner->SetPosition(m_kinematicController->GetPosition());
    }

    glm::quat PlayerControllerComponent::CreateQuaternion(glm::vec3 direction, float angle)
    {
        glm::vec3 normal = glm::normalize(direction);
        float W = std::cos(angle / 2);
        float X = std::sin(angle / 2) * normal.x;
        float Y = std::sin(angle / 2) * normal.y;
        float Z = std::sin(angle / 2) * normal.z;
        return glm::quat(W, X, Y, Z);
    }
}
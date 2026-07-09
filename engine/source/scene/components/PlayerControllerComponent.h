#pragma once
#include "scene/Component.h"
#include "physics/KinematicCharacterController.h"
#include <memory>

namespace CEngine
{
    class PlayerControllerComponent : public Component
    {
        COMPONENT(PlayerControllerComponent)
        public:
            void Init() override;
            void Update(float deltaTime) override;

        private:
            float m_sensitivity = 0.03f;
            float m_moveSpeed = 1.0f;

            float m_xRot = 0.0f;
            float m_yRot = 0.0f;
            std::unique_ptr<KinematicCharacterController> m_kinematicController;

            glm::quat CreateQuaternion(glm::vec3 direction, float angle);
    };
}
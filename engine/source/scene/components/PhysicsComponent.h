#pragma once
#include "scene/Component.h"
#include "physics/RigidBody.h"
#include <memory>

namespace CEngine
{
    class PhysicsComponent : public Component
    {
        COMPONENT(PhysicsComponent)
        public:
            PhysicsComponent() = default;
            PhysicsComponent(std::shared_ptr<RigidBody> &body);

            void Init() override;
            void Update(float deltaTime) override;

            void SetRigidBody(std::shared_ptr<RigidBody> &body);

        private:
            std::shared_ptr<RigidBody> m_rigidBody;
    };
}
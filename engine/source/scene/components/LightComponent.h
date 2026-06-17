#pragma once
#include "scene/Component.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace CEngine
{
    class LightComponent : public Component
    {
        COMPONENT(LightComponent)
        public:
            void Update(float deltaTime) override;

            const glm::vec3& GetColor() const;
            void SetColor(const glm::vec3 &color);

        private:
            glm::vec3 m_color = glm::vec3(1.0f);

    };
}
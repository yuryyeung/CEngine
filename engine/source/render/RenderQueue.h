#pragma once
#include "render/Mesh.h"
#include "render/Material.h"
#include "graphics/GraphicsAPI.h"
#include <vector>
#include <glm/mat4x4.hpp>

namespace CEngine
{
    struct RenderCommand
    {
        Mesh *mesh = nullptr;
        Material *material = nullptr;
        glm::mat4 modelMatrix;
    };

    struct CameraData
    {
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    };

    class RenderQueue
    {
        public:
            void Submit(const RenderCommand& command);   // Enqueue Draw Command
            void Draw(GraphicsAPI& graphicesAPI, const CameraData& cameraData);

        private:
            std::vector<RenderCommand> m_commands;
    };
}
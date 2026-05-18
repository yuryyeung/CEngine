#pragma once
#include "render/Mesh.h"
#include "render/Material.h"
#include "graphics/GraphicsAPI.h"
#include <vector>

namespace CEngine
{
    struct RenderCommand
    {
        Mesh *mesh = nullptr;
        Material *material = nullptr;
    };

    class RenderQueue
    {
        public:
            void Submit(const RenderCommand& command);   // Enqueue Draw Command
            void Draw(GraphicsAPI& graphicesAPI);

        private:
            std::vector<RenderCommand> m_commands;
    };
}
#include "render/RenderQueue.h"
#include "render/Mesh.h";
#include "render/Material.h";
#include "graphics/GraphicsAPI.h"

namespace CEngine
{
    void RenderQueue::Submit(const RenderCommand &command) 
    {
        m_commands.push_back(command);
    }

    void RenderQueue::Draw(GraphicsAPI &graphicesAPI)
    {
        for (auto& command : m_commands)
        {
            graphicesAPI.BindMaterial(command.material);
            graphicesAPI.BindMesh(command.mesh);
            graphicesAPI.DrawMesh(command.mesh);
        }

        m_commands.clear();
    }
}
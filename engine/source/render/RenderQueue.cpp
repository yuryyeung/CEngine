#include "render/RenderQueue.h"
#include "render/Mesh.h";
#include "render/Material.h";
#include "graphics/GraphicsAPI.h"
#include "graphics/ShaderProgram.h"

namespace CEngine
{
    void RenderQueue::Submit(const RenderCommand &command) 
    {
        m_commands.push_back(command);
    }

    void RenderQueue::Draw(GraphicsAPI &graphicesAPI, const CameraData &cameraData)
    {
        for (auto& command : m_commands)
        {
            graphicesAPI.BindMaterial(command.material);
            auto shaderProgram = command.material->GetShaderProgram();
            shaderProgram->SetUniform("uModel", command.modelMatrix);
            shaderProgram->SetUniform("uView", cameraData.viewMatrix);
            shaderProgram->SetUniform("uProjection", cameraData.projectionMatrix);

            graphicesAPI.BindMesh(command.mesh);
            graphicesAPI.DrawMesh(command.mesh);
        }

        m_commands.clear();
    }
}
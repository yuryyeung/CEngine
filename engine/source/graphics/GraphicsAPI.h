#pragma once
#include <memory>

namespace CEngine
{
    class ShaderProgram;
    class Material;
    class GraphicsAPI
    {
        public:
            std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
            void BindShaderProgram(ShaderProgram* shaderProgram);
            void BindMaterial(Material* material);
    };
} 

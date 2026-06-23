#pragma once
#include <memory>
#include <string>
#include <vector>
#include <GL/glew.h>

namespace CEngine
{
    class ShaderProgram;
    class Material;
    class Mesh;

    class GraphicsAPI
    {
        public:
            bool Init();
            std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
            void BindShaderProgram(ShaderProgram* shaderProgram);
            void BindMaterial(Material* material);
            void BindMesh(Mesh *mesh);
            void DrawMesh(Mesh *mesh);

            void SetClearColor(float r, float g, float b, float a);
            void ClearBuffers();

            GLuint CreateVertexBuffer(const std::vector<float> &vertices);
            GLuint CreateIndexBuffer(const std::vector<uint32_t> &indices);

            const std::shared_ptr<ShaderProgram>& GetDefaultShaderProgram();

        private:
            std::shared_ptr<ShaderProgram> m_defaultShaderProgram;
    };
} 

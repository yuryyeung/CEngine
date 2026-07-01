#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include "graphics/VertexLayout.h"

namespace CEngine
{
    class Mesh
    {
        public:
            Mesh(const VertexLayout &layout, const std::vector<float> &vertices, const std::vector<uint32_t> &indices);
            Mesh(const VertexLayout &layout, const std::vector<float> &vertices);

            Mesh(const Mesh &) = delete;
            Mesh &operator=(const Mesh&) = delete;

            void Bind();
            void Draw();

            // static std::shared_ptr<Mesh> Load(const std::string &path);
            static std::shared_ptr<Mesh> CreateCube(const glm::vec3 &size = glm::vec3(1.0f));
            static std::shared_ptr<Mesh> CreateBox();

        private:
            VertexLayout m_vertexLayout;

            GLuint m_VBO = 0;
            GLuint m_EBO = 0;
            GLuint m_VAO = 0;

            size_t m_vertexCount = 0;
            size_t m_indexCount = 0;
        };
}
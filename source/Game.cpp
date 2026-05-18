#include "Game.h"
#include <CEngine.h>
#include <iostream>
#include <GLFW/glfw3.h>

bool Game::Init()
{
    // Vertex Shader
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        uniform vec2 uOffset;

        out vec3 vColor;

        void main()
        {
            vColor = color;
            gl_Position = vec4(position.x + uOffset.x, position.y + uOffset.y, position.z, 1.0);
        }
    )";

    // Fragment Shader
    std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 vColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0);
        }
    )";

    auto& graphicsAPI = CEngine::Engine::GetInstance().GetGraphicsAPI();
    auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
    m_material.SetShaderProgram(shaderProgram);

    std::vector<float> vertices =
        {
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f};

    std::vector<unsigned int> indices =
        {
            0, 1, 2,
            0, 2, 3};

    CEngine::VertexLayout vertexLayout;

    // Position
    vertexLayout.elements.push_back({0, 3, GL_FLOAT, 0});

    // Color
    vertexLayout.elements.push_back({1, 3, GL_FLOAT, sizeof(float) * 3});
    vertexLayout.stride = sizeof(float) * 6;

    m_mesh = std::make_unique<CEngine::Mesh>(vertexLayout, vertices, indices);

    return true;
}

void Game::Update(float deltaTime)
{
    // Debug::Log("Current Delta: " + std::to_string(deltaTime));
    auto &input = CEngine::Engine::GetInstance().GetInputManager();
    if (input.IsKeyPressed(GLFW_KEY_A))
    {
        Debug::Log("[A] Button is pressed");
    }

    CEngine::RenderCommand command;
    command.material = &m_material;
    command.mesh = m_mesh.get();

    auto &renderQueue = CEngine::Engine::GetInstance().GetRenderQueue();
    renderQueue.Submit(command);
}

void Game::Destroy()
{

}
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
        uniform vec4 uColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0) * uColor;
        }
    )";

    auto& graphicsAPI = CEngine::Engine::GetInstance().GetGraphicsAPI();
    auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

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
}

void Game::Destroy()
{

}
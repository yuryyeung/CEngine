#include "TestObject.h"
#include <GLFW/glfw3.h>

TestObject::TestObject()
{
    // Vertex Shader
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        uniform mat4 uModel;

        out vec3 vColor;

        void main()
        {
            vColor = color;
            gl_Position = uModel * vec4(position, 1.0);
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

    auto &graphicsAPI = CEngine::Engine::GetInstance().GetGraphicsAPI();
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

    m_mesh = std::make_shared<CEngine::Mesh>(vertexLayout, vertices, indices);
}

void TestObject::Update(float deltaTime)
{
    CEngine::GameObject::Update(deltaTime);
    // Debug::Log("Current Delta: " + std::to_string(deltaTime));
    auto position = GetPosition();
    auto &input = CEngine::Engine::GetInstance().GetInputManager();
    if (input.IsKeyPressed(GLFW_KEY_A))
    {
        Debug::Log("[A] Button is pressed");
        position.x -= 1 * deltaTime;
    }
    if (input.IsKeyPressed(GLFW_KEY_D))
    {
        Debug::Log("[D] Button is pressed");
        position.x += 1 * deltaTime;
    }
    if (input.IsKeyPressed(GLFW_KEY_W))
    {
        Debug::Log("[W] Button is pressed");
        position.y += 1 * deltaTime;
    }
    if (input.IsKeyPressed(GLFW_KEY_S))
    {
        Debug::Log("[S] Button is pressed");
        position.y -= 1 * deltaTime;
    }
    SetPosition(position);

    CEngine::RenderCommand command;
    command.material = &m_material;
    command.mesh = m_mesh.get();
    command.modelMatrix = GetWorldTransform();

    auto &renderQueue = CEngine::Engine::GetInstance().GetRenderQueue();
    renderQueue.Submit(command);
}
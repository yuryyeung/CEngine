#include "TestObject.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TestObject::TestObject()
{
    auto &fs = CEngine::Engine::GetInstance().GetFileSystem();
    auto path = fs.GetAssetsFolder() / "brick.png";

    int width, height, channels;
    unsigned char *data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    std::shared_ptr<CEngine::Texture> texture;
    if (data)
    {
        texture = std::make_shared<CEngine::Texture>(width, height, channels, data);
        Debug::Log("Image Loaded");
        stbi_image_free(data);
    }

    // Vertex Shader
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;
        layout (location = 2) in vec2 uv;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;

        out vec3 vColor;
        out vec2 vUV;

        void main()
        {
            vColor = color;
            vUV = uv;
            gl_Position = uProjection * uView * uModel * vec4(position, 1.0);
        }
    )";

    // Fragment Shader
    std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 vColor;
        in vec2 vUV;

        uniform sampler2D brickTexture;

        void main()
        {
            vec4 texColor = texture(brickTexture, vUV);
            FragColor = texColor * vec4(vColor, 1.0);
        }
    )";

    auto &graphicsAPI = CEngine::Engine::GetInstance().GetGraphicsAPI();
    auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);    

    auto material = std::make_shared<CEngine::Material>();
    material->SetShaderProgram(shaderProgram);
    material->SetParam("brickTexture", texture);

    std::vector<float> vertices =
        {
            // Front face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

            // Top face
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

            // Right face
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

            // Left face
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

            // Bottom face
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

            // Back face
            -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    std::vector<unsigned int> indices =
        {
            // front face
            0, 1, 2,
            0, 2, 3,
            // top face
            4, 5, 6,
            4, 6, 7,
            // right face
            8, 9, 10,
            8, 10, 11,
            // left face
            12, 13, 14,
            12, 14, 15,
            // bottom face
            16, 17, 18,
            16, 18, 19,
            // back face
            20, 21, 22,
            20, 22, 23};

    CEngine::VertexLayout vertexLayout;

    // Position
    vertexLayout.elements.push_back({0, 3, GL_FLOAT, 0});

    // Color
    vertexLayout.elements.push_back({1, 3, GL_FLOAT, sizeof(float) * 3});

    // UV
    vertexLayout.elements.push_back({2, 2, GL_FLOAT, sizeof(float) * 6});

    vertexLayout.stride = sizeof(float) * 8;

    auto mesh = std::make_shared<CEngine::Mesh>(vertexLayout, vertices, indices);
    AddComponent(new CEngine::MeshComponent(material, mesh));
}

void TestObject::Update(float deltaTime)
{
    CEngine::GameObject::Update(deltaTime);

#if 0
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
#endif
}
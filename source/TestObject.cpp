#include "TestObject.h"
#include <GLFW/glfw3.h>

TestObject::TestObject()
{
    auto material = CEngine::Material::Load("materials/brick.mat");
    auto mesh = CEngine::Mesh::CreateCube();
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
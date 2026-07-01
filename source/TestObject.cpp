#include "TestObject.h"
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <time.h>

TestObject::TestObject()
{

    srand(time(NULL));
    float min = -8.0f;
    float max = 8.0f;
    float x = (max - min) * rand() / (RAND_MAX + 1.0) + min;
    float y = (max - min) * rand() / (RAND_MAX + 1.0) + min;
    float z = (max - min) * rand() / (RAND_MAX + 1.0) + min;

    this->SetPosition(glm::vec3(x, y + 10.0f, z));
    this->SetRotation(glm::quat(glm::vec3(x, y, z)));

    auto material = CEngine::Material::Load("materials/brick.mat");
    auto mesh = CEngine::Mesh::CreateCube();
    AddComponent(new CEngine::MeshComponent(material, mesh));

    auto boxCollider = std::make_shared<CEngine::BoxCollider>(this->GetScale());
    auto boxBody = std::make_shared<CEngine::RigidBody>(CEngine::BodyType::Dynamic, boxCollider, 5.0f, 0.5f);
    AddComponent(new CEngine::PhysicsComponent(boxBody));
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
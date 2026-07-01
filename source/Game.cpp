#include <iostream>
#include <CEngine.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "TestObject.h"

bool Game::Init()
{
    m_scene = new CEngine::Scene();
    CEngine::Engine::GetInstance()
        .SetScene(m_scene);

    auto camera = m_scene->CreateObject("Camera");
    camera->AddComponent(new CEngine::CameraComponent());
    camera->SetPosition(glm::vec3(0.0f, 1.0f, 7.0f));
    camera->AddComponent(new CEngine::PlayerControllerComponent());

    m_scene->SetMainCamera(camera);
    m_scene->CreateObject<TestObject>("TestObject");

    for (int i = 0; i < 10; i++)
    {
        TestObject* gameobject = m_scene->CreateObject<TestObject>("TestObject_" + i);
    }

    // auto suzanneMesh = CEngine::Mesh::Load("models/Suzanne.gltf");
    // auto suzanneMaterial = CEngine::Material::Load("materials/suzanne.mat");

    // auto suzzanneObj = m_scene->CreateObject("Suzanne");
    // suzzanneObj->AddComponent(new CEngine::MeshComponent(suzanneMaterial, suzanneMesh));

    auto suzzanneObj = CEngine::GameObject::LoadGLTF("models/suzanne/Suzanne.gltf");
    suzzanneObj->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

    auto gun = CEngine::GameObject::LoadGLTF("models/sten_gunmachine_carbine/scene.gltf");
    gun->SetParent(camera);
    gun->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
    gun->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

    if (auto anim = gun->GetComponent<CEngine::AnimationComponent>())
    {
        if (auto bullet = gun->FindChildByName("bullet_33"))
        {
            // bullet->SetActive(false);
        }

        if (auto fire = gun->FindChildByName("BOOM_35"))
        {
            // fire->SetActive(false);
        }
        anim->Play("shoot");
    }

    auto light = m_scene->CreateObject("Light");
    auto lightComp = new CEngine::LightComponent();
    light->AddComponent(lightComp);
    light->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));

    auto ground = m_scene->CreateObject("Ground");
    ground->SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));

    glm::vec3 groundExtents = glm::vec3(20.0f, 2.0f, 20.0f);
    auto groundMesh = CEngine::Mesh::CreateCube(groundExtents);
    auto material = CEngine::Material::Load("materials/brick.mat");
    ground->AddComponent(new CEngine::MeshComponent(material, groundMesh));

    auto groundCollider = std::make_shared<CEngine::BoxCollider>(groundExtents);
    auto groundBody = std::make_shared<CEngine::RigidBody>(CEngine::BodyType::Static, groundCollider, 0.0f, 0.5f);
    ground->AddComponent(new CEngine::PhysicsComponent(groundBody));

    return true;
}

void Game::Update(float deltaTime)
{
    m_scene->Update(deltaTime);
}

void Game::Destroy()
{

}
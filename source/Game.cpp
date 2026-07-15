#include <iostream>
#include <CEngine.h>
#include "Game.h"
#include "TestObject.h"
#include "Player.h"

void Game::RegisterTypes()
{
    Player::Register();
}

bool Game::Init()
{
    m_scene = new CEngine::Scene();
    CEngine::Engine::GetInstance()
        .SetScene(m_scene);

    auto player = m_scene->CreateObject<Player>("Player");
    player->Init();
    m_scene->SetMainCamera(player);
    m_scene->CreateObject<TestObject>("TestObject");

    for (int i = 0; i < 1; i++)
    {
        TestObject* gameobject = m_scene->CreateObject<TestObject>("TestObject_" + i);
    }

    // auto suzanneMesh = CEngine::Mesh::Load("models/Suzanne.gltf");
    // auto suzanneMaterial = CEngine::Material::Load("materials/suzanne.mat");

    // auto suzzanneObj = m_scene->CreateObject("Suzanne");
    // suzzanneObj->AddComponent(new CEngine::MeshComponent(suzanneMaterial, suzanneMesh));

    auto suzzanneObj = CEngine::GameObject::LoadGLTF("models/suzanne/Suzanne.gltf");
    suzzanneObj->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

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
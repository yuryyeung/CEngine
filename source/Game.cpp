#include <iostream>
#include <CEngine.h>
#include <time.h>
#include "Game.h"
#include "TestObject.h"

bool Game::Init()
{
    m_scene = new CEngine::Scene();

    auto camera = m_scene->CreateObject("Camera");
    camera->AddComponent(new CEngine::CameraComponent());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new CEngine::PlayerControllerComponent());

    m_scene->SetMainCamera(camera);

    m_scene->CreateObject<TestObject>("TestObject");

    srand(time(NULL));
    float min = -2.0f;
    float max = 2.0f;
    float spawns = 10;

    for (int i = 0; i < spawns; i++)
    {
        float x = (max - min) * rand() / (RAND_MAX + 1.0) + min;
        float y = (max - min) * rand() / (RAND_MAX + 1.0) + min;
        float z = (max - min) * rand() / (RAND_MAX + 1.0) + min;

        TestObject* gameobject = m_scene->CreateObject<TestObject>("TestObject_" + i);
        gameobject->SetPosition(glm::vec3(x, y, z));
    }

    CEngine::Engine::GetInstance()
        .SetScene(m_scene);
    return true;
}

void Game::Update(float deltaTime)
{
    m_scene->Update(deltaTime);
}

void Game::Destroy()
{

}
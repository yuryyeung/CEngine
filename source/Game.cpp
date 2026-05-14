#include "Game.h"
#include <CEngine.h>
#include <iostream>
#include <GLFW/glfw3.h>

bool Game::Init()
{
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
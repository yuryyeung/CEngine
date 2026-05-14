#include "Game.h"
#include <CEngine.h>
#include <iostream>

bool Game::Init()
{
    return true;
}

void Game::Update(float deltaTime)
{
    Debug::Log("Current Delta: " + std::to_string(deltaTime));
}

void Game::Destroy()
{

}
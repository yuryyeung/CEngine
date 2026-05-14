#pragma once
#include <CEngine.h>

class Game : public CEngine::Application
{
public:
    bool Init() override;
    void Update(float deltaTime) override;
    void Destroy() override;
};
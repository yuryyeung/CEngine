#pragma once
#include <CEngine.h>
#include <memory>

class Game : public CEngine::Application
{
    public:
        bool Init() override;
        void Update(float deltaTime) override;
        void Destroy() override;

    private:
        CEngine::Scene* m_scene = nullptr;
};
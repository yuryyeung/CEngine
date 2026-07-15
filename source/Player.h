#pragma once
#include <CEngine.h>

class Player : public CEngine::GameObject
{
    GAMEOBJECT(Player)
    public:
        void Init();
        void Update(float deltaTime) override;

    private:
        CEngine::AnimationComponent *m_animationComponent = nullptr;
};
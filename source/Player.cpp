#include "Player.h"
#include <CEngine.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

CEngine::GameObject* gun;

void Player::Init()
{
    SetActive(true);
    AddComponent(new CEngine::CameraComponent());
    SetPosition(glm::vec3(0.0f, 1.0f, 7.0f));
    AddComponent(new CEngine::PlayerControllerComponent());

    gun = CEngine::GameObject::LoadGLTF("models/sten_gunmachine_carbine/scene.gltf");
    gun->SetParent(this);
    gun->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
    gun->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

    m_animationComponent = gun->GetComponent<CEngine::AnimationComponent>();
}

void Player::Update(float deltaTime)
{
    CEngine::GameObject::Update(deltaTime);
    auto &inputManager = CEngine::Engine::GetInstance().GetInputManager();

    if (inputManager.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        if (m_animationComponent && !m_animationComponent->IsPlaying())
        {
            if (auto anim = m_animationComponent)
            {
                if (auto bullet = gun->FindChildByName("bullet_33"))
                {
                    // bullet->SetActive(false);
                }

                if (auto fire = gun->FindChildByName("BOOM_35"))
                {
                    // fire->SetActive(false);
                }
                anim->Play("shoot", false);
            }
        }
    }
}
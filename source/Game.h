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
        CEngine::Material m_material;
        std::unique_ptr<CEngine::Mesh> m_mesh;
};
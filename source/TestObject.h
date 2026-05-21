#pragma once

#include <CEngine.h>

class TestObject : public CEngine::GameObject
{
    public:
        TestObject();
        void Update(float deltaTime) override;

    private:
        CEngine::Material m_material;
        std::shared_ptr<CEngine::Mesh> m_mesh;
};
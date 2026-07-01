#pragma once

#include <CEngine.h>
#include <glm/vec3.hpp>

class TestObject : public CEngine::GameObject
{
    public:
        TestObject();
        void Update(float deltaTime) override;

    private:
};
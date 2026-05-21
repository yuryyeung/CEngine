#pragma once

#include <CEngine.h>

class TestObject : public CEngine::GameObject
{
    public:
        TestObject();
        void Update(float deltaTime) override;

    private:
};
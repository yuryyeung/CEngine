#pragma once
#include <cstddef>

namespace CEngine
{
    class GameObject;
    class Component
    {
        public:
            virtual ~Component() = default;
            virtual void Init();
            virtual void Update(float deltaTime) = 0;
            virtual size_t GetTypeId() const = 0;

            GameObject *GetOwner();

            template <typename T>
            static size_t StaticTypeId()
            {
                static size_t typeId = nextId++;
                return typeId;
            }

        protected:
            GameObject *m_owner = nullptr;
            friend class GameObject;

        private:
            static size_t nextId;
    };

    class ComponentCreatorBase
    {
        public:
            virtual ~ComponentCreatorBase() = default;
            virtual Component *CreateComponent() = 0;
    };

    template <typename T>
    class ComponentCreator : public ComponentCreatorBase
    {
        public:
            Component* CreateComponent() override
            {
                return new T();
            }
    };

    class ComponentFactory
    {
        public:
            static ComponentFactory& GetInstance();
    }

#define COMPONENT(ComponentClass)                                                \
public:                                                                          \
    static size_t TypeId() { return Component::StaticTypeId<ComponentClass>(); } \
    size_t GetTypeId() const override { return TypeId(); }                       
}
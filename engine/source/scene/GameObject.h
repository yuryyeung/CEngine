#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "scene/Component.h"
#include <glm/gtc/quaternion.hpp>
#include <nlohmann/json.hpp>

namespace CEngine
{
    class Scene;
    class GameObject
    {
        public:
            virtual ~GameObject() = default;
            virtual void Init();
            virtual void LoadProperties(const nlohmann::json& json);
            virtual void Update(float deltaTime);
            const std::string &GetName() const;
            void SetName(const std::string &name);
            GameObject *GetParent();
            bool SetParent(GameObject* parent);
            Scene *GetScene();
            bool isAlive() const;
            void MarkForDestory();
            
            void SetActive(bool value);
            bool IsActive() const;

            void AddComponent(Component* component);
            template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<Component, T>>>
            T* GetComponent()
            {
                size_t typeId = Component::StaticTypeId<T>();
                for (auto& component : m_components)
                {
                    if (component->GetTypeId() == typeId)
                    {
                        return static_cast<T *>(component.get());
                    }
                }
                return nullptr;
            }

            GameObject* FindChildByName(const std::string &name);

            const glm::vec3& GetPosition() const;
            glm::vec3 GetWorldPosition() const;
            void SetPosition(const glm::vec3 &pos);
            void SetWorldPosition(const glm::vec3 &pos);

            const glm::quat& GetRotation() const;
            glm::quat GetWorldRotation() const;
            void SetRotation(const glm::quat &rot);
            void SetWorldRotation(const glm::quat &rot);

            const glm::vec3& GetScale() const;
            void SetScale(const glm::vec3 &scale);

            glm::mat4 GetLocalTransform() const;
            glm::mat4 GetWorldTransform() const;

            static GameObject *LoadGLTF(const std::string &path);


        protected:
            GameObject() = default;
            
            std::string m_name;
            GameObject *m_parent = nullptr;
            Scene *m_scene = nullptr;
            std::vector<std::unique_ptr<GameObject>> m_children;
            std::vector<std::unique_ptr<Component>> m_components;
            bool m_isAlive = true;
            glm::vec3 m_position = glm::vec3(0.0f);
            glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            glm::vec3 m_scale = glm::vec3(1.0f);
            bool m_isActive = true;

            friend class Scene;
    };

    class ObjectCreatorBase
    {
        public:
            virtual ~ObjectCreatorBase() = default;
            virtual GameObject* CreateGameObject(const std::string& typeName) = 0;
    };

    template <typename T>
    class ObjectCreator : public ObjectCreatorBase
    {
        public:
            virtual GameObject* CreateGameObject override()
            {
                return new ();
            }
    };

    class GameObjectFactory
    {
        public:
            static GameObjectFactory& GetInstance();
            template <typename T>
            void RegisterObject(const std::string& name)
            {
                m_creators.emplace(name, std::make_unique<ObjectCreator<T>>());
            }
    };

#define GAMEOBJECT(ObjectClass) \
    public:                     \
        static void Register() { CEngine::GameObjectFactory::GetInstance().RegisterObject<ObjectClass>(std::string(#ObjectClass)); }
}
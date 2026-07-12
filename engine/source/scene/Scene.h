#pragma once
#include <vector>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "GameObject.h"
#include "Common.h"

namespace CEngine
{
    class Scene
    {   
        private:
            std::vector<std::unique_ptr<GameObject>> m_objects;
            GameObject *m_mainCamera = nullptr;
            void CollectLightRecursive(GameObject* obj, std::vector<LightData>& out);
            void LoadObject(const nlohmann::json &jsonObject, GameObject *parent);

        public:
            void Update(float deltaTime);
            void Clear();

            GameObject *CreateObject(const std::string &name, GameObject *parent = nullptr);

            template <typename T, typename = typename std::enable_if_t<std::is_base_of_v<GameObject, T>>>
            T *CreateObject(const std::string &name, GameObject *parent = nullptr)
            {
                auto obj = new T();
                obj->SetName(name);
                SetParent(obj, parent);
                obj->m_scene = this;
                return obj;
            }

            bool SetParent(GameObject* gameObject, GameObject* parent);

            void SetMainCamera(GameObject* mainCamera);
            GameObject *GetMainCamera();

            std::vector<LightData> CollectLights();

            static std::shared_ptr<Scene> Load(const std::string &path);
    };
}
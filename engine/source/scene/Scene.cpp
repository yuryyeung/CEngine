#include "Engine.h"
#include "utils/Debug.h"
#include "scene/Scene.h"
#include "scene/GameObject.h"
#include "scene/components/AnimationComponent.h"
#include "scene/components/CameraComponent.h"
#include "scene/components/LightComponent.h"
#include "scene/components/MeshComponent.h"
#include "scene/components/PhysicsComponent.h"
#include "scene/components/PlayerControllerComponent.h"
#include <algorithm>

namespace CEngine
{
    void Scene::RegisterTypes()
    {
        AnimationComponent::Register();
        CameraComponent::Register();
        LightComponent::Register();
        MeshComponent::Register();
        PhysicsComponent::Register();
        PlayerControllerComponent::Register();
    }

    void Scene::Update(float deltaTime)
    {
        for (auto it = m_objects.begin(); it != m_objects.end();)
        {
            if ((*it)->isAlive())
            {
                // Debug::Log("IT Update:" + (*it).get()->GetName() + " @ Scene");
                (*it)->Update(deltaTime);
                ++it;
            }
            else
            {
                it = m_objects.erase(it);
            }
        }
    }

    void Scene::Clear()
    {
        m_objects.clear();
    }

    GameObject* Scene::CreateObject(const std::string& name, GameObject* parent)
    {
        auto gameObject = new GameObject();
        gameObject->SetName(name);
        gameObject->m_scene = this;
        SetParent(gameObject, parent);
        return gameObject;
    }

    bool Scene::SetParent(GameObject *obj, GameObject *parent)
    {
        bool result = false;
        auto currentParent = obj->GetParent();

        if (parent == nullptr)
        {
            if (currentParent != nullptr)
            {
                auto it = std::find_if(
                    currentParent->m_children.begin(),
                    currentParent->m_children.end(),
                    [obj](const std::unique_ptr<GameObject> &el)
                    {
                        return el.get() == obj;
                    });

                if (it != currentParent->m_children.end())
                {
                    m_objects.push_back(std::move(*it));
                    obj->m_parent = nullptr;
                    currentParent->m_children.erase(it);
                    result = true;
                }
            }
            // No parent currently. This can be in 2 cases
            // 1. The object is in the scene root
            // 2. The object has been just created
            else
            {
                auto it = std::find_if(
                    m_objects.begin(),
                    m_objects.end(),
                    [obj](const std::unique_ptr<GameObject> &el)
                    {
                        return el.get() == obj;
                    });

                if (it == m_objects.end())
                {
                    std::unique_ptr<GameObject> objHolder(obj);
                    m_objects.push_back(std::move(objHolder));
                    result = true;
                }
            }
        }
        // We are trying to add it as a child of another object
        else
        {
            if (currentParent != nullptr)
            {
                auto it = std::find_if(
                    currentParent->m_children.begin(),
                    currentParent->m_children.end(),
                    [obj](const std::unique_ptr<GameObject> &el)
                    {
                        return el.get() == obj;
                    });

                if (it != currentParent->m_children.end())
                {
                    bool found = false;
                    auto currentElement = parent;
                    while (currentElement)
                    {
                        if (currentElement == obj)
                        {
                            found = true;
                            break;
                        }
                        currentElement = currentElement->GetParent();
                    }

                    if (!found)
                    {
                        parent->m_children.push_back(std::move(*it));
                        obj->m_parent = parent;
                        currentParent->m_children.erase(it);
                        result = true;
                    }
                }
            }
            // No parent currently. This can be in 2 cases
            // 1. The object is in the scene root
            // 2. The object has been just created
            else
            {
                auto it = std::find_if(
                    m_objects.begin(),
                    m_objects.end(),
                    [obj](const std::unique_ptr<GameObject> &el)
                    {
                        return el.get() == obj;
                    });

                // The object has been hust created
                if (it == m_objects.end())
                {
                    std::unique_ptr<GameObject> objHolder(obj);
                    parent->m_children.push_back(std::move(objHolder));
                    obj->m_parent = parent;
                    result = true;
                }
                else
                {
                    bool found = false;
                    auto currentElement = parent;
                    while (currentElement)
                    {
                        if (currentElement == obj)
                        {
                            found = true;
                            break;
                        }
                        currentElement = currentElement->GetParent();
                    }

                    if (!found)
                    {
                        parent->m_children.push_back(std::move(*it));
                        obj->m_parent = parent;
                        m_objects.erase(it);
                        result = true;
                    }
                }
            }
        }

        return result;
    }

    void Scene::SetMainCamera(GameObject *camera)
    {
        m_mainCamera = camera;
    }

    GameObject *Scene::GetMainCamera()
    {
        return m_mainCamera;
    }

    std::vector<LightData> Scene::CollectLights()
    {
        std::vector<LightData> lights;
        for (auto& obj : m_objects)
        {
            CollectLightRecursive(obj.get(), lights);
        }
        return lights;
    }

    void Scene::CollectLightRecursive(GameObject* obj, std::vector<LightData>& out)
    {
        if (auto light = obj->GetComponent<LightComponent>())
        {
            LightData data;
            data.color = light->GetColor();
            data.position = obj->GetPosition();     
            out.push_back(data);   
        }

        for (auto& child : obj->m_children)
        {
            CollectLightRecursive(child.get(), out);
        }
    }

    std::shared_ptr<Scene> Scene::Load(const std::string &path)
    {
        const std::string contents = Engine::GetInstance().GetFileSystem().LoadAssetFileText(path);
        if (contents.empty())
        {
            return nullptr;
        }

        auto json = nlohmann::json::parse(contents);
        if (json.empty())
        {
            return nullptr;
        }

        auto result = std::make_shared<Scene>();

        const std::string sceneName = json.value("name", "noname");
        if (json.contains("object") && json["object"].is_array())
        {
            const auto &objects = json["object"];
            for (const auto& obj : objects)
            {
                result->LoadObject(obj, nullptr);
            }
        }

        return result;
    }

    void Scene::LoadObject(const nlohmann::json &jsonObject, GameObject *parent)
    {
        const std::string name = jsonObject.value("name", "Object");
        GameObject* gameObject = nullptr;

        if (jsonObject.contains("type"))
        {
            const std::string type = jsonObject.value("type", "");
        }
        else
        {
            gameObject = CreateObject("name", parent);
        }

        if (!gameObject)
        {
            return;
        }

        // Read Transform
        if (jsonObject.contains("position"))
        {
            auto posObj = jsonObject["position"];
            glm::vec3 pos = glm::vec3(
                posObj.value("x", 0.0f), posObj.value("y", 0.0f), posObj.value("z", 0.0f));
            gameObject->SetPosition(pos);
        }

        if (jsonObject.contains("rotation"))
        {
            auto rotObj = jsonObject["rotation"];
            glm::quat rot = glm::quat(
                rotObj.value("x", 0.0f), rotObj.value("y", 0.0f), rotObj.value("z", 0.0f), rotObj.value("w", 1.0f));
            gameObject->SetRotation(rot);
        }

        if (jsonObject.contains("scale"))
        {
            auto scaleObj = jsonObject["scale"];
            glm::vec3 scale = glm::vec3(
                scaleObj.value("x", 0.0f), scaleObj.value("y", 0.0f), scaleObj.value("z", 0.0f));
            gameObject->SetScale(scale);
        }

        if (jsonObject.contains("components") && jsonObject["components"].is_array())
        {
            const auto &components = jsonObject["components"];
            for (const auto& comp : components)
            {
                const std::string type = comp.value("type", "");
                Component* component = ComponentFactory::GetInstance().CreateComponent(type);
                if (component)
                {
                    component->LoadProperties(comp);
                    gameObject->AddComponent(component);
                }
            }
        }
    }
}
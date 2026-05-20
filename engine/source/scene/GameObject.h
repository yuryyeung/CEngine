#pragma once
#include <string>
#include <vector>
#include <memory>

namespace CEngine
{
    class GameObject
    {
        public:
            virtual ~GameObject() = default;
            virtual void Update(float deltaTime);
            const std::string &GetName() const;
            void SetName(const std::string &name);
            GameObject *GetParent();
            bool isAlive() const;
            void MarkForDestory();

        protected:
            GameObject() = default;

        private:
            std::string m_name;
            long m_id;
            GameObject *m_parent = nullptr;
            std::vector<std::unique_ptr<GameObject>> m_children;
            bool m_isAlive = true;

            friend class Scene;
    };
}
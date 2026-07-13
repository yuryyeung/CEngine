#pragma once

namespace CEngine
{
    class Application
    {
        public:
            virtual ~Application() = default;
            virtual void RegisterTypes();
            virtual bool Init() = 0;
            virtual void Update(float deltaTime) = 0;
            virtual void Destroy() = 0;

            void SetNeedToBeClosed(bool value);
            bool NeedToBeClosed() const;

        private:
            bool m_needsToBeClosed = false;
    };
}
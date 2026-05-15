#include "Engine.h"
#include "Application.h"
#include "utils/Debug.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace CEngine
{
    void keyCallback(GLFWwindow* window, int key, int, int action, int)
    {
        auto &inputManager = CEngine::Engine::GetInstance().GetInputManager();
        if(action == GLFW_PRESS)
        {
            inputManager.SetKeyPressed(key, true);
        }
        else if (action == GLFW_RELEASE)
        {
            inputManager.SetKeyPressed(key, false);
        }
    }

    Engine &Engine::GetInstance()
    {
        static Engine instance;
        return instance;
    }

    bool Engine::Init(int width, int height)
    {
        if (!m_application)
        {
            return false;
        }

        if (!glfwInit())
        {
            Debug::LogError("GLFW NOT BEEN INITIALIZED");
            return false;
        }

        // Define OpenGL and GLFW content version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Define GLFWwindow size and create
        m_window = glfwCreateWindow(width, height, "CEngine", nullptr, nullptr);
        Debug::Log("Start Create Window");

        if (m_window == nullptr)
        {
            Debug::LogError("Error creating window");
            glfwTerminate();
            return false;
        }

        glfwSetKeyCallback(m_window, keyCallback);

        // Show Window
        glfwMakeContextCurrent(m_window);

        if (glewInit() != GLEW_OK)
        {
            Debug::LogError("GLEW is not ready");
            glfwTerminate();
            return false;
        }

        return m_application->Init();
    }

    void Engine::Run()
    {
        if (!m_application)
        {
            return;
        }

#if defined(__linux__)
        m_lastTimePoint == std::chrono::steady_clock::now();
#elif defined(Win32)
        m_lastTimePoint == std::chrono::high_resolution_clock::now();
#endif
        while(!glfwWindowShouldClose(m_window) && !m_application -> NeedToBeClosed())
        {
            glfwPollEvents();

#if defined(__linux__)
            auto now = std::chrono::steady_clock::now();
#elif defined(Win32)
            auto now = std::chrono::high_resolution_clock::now();
#endif
            float deltaTime = std::chrono::duration<float>(now - m_lastTimePoint).count();
            m_lastTimePoint = now;

            m_application->Update(deltaTime);

            glfwSwapBuffers(m_window);
        }
    }

    void Engine::Destroy()
    {
        if (m_application)
        {
            m_application->Destroy();
            m_application.reset();
            glfwTerminate();
            m_window = nullptr;
        }
    }

    void Engine::SetApplication(Application *app)
    {
        m_application.reset(app);
    }

    Application *Engine::GetApplication()
    {
        return m_application.get();
    }

    InputManager &Engine::GetInputManager()
    {
        return m_inputManager;
    }

    GraphicsAPI &Engine::GetGraphicsAPI()
    {
        return m_graphicsAPI;
    }
}
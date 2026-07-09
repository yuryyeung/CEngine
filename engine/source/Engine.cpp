#include "Engine.h"
#include "Application.h"
#include "utils/Debug.h"
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene/GameObject.h"
#include "scene/Component.h"
#include "scene/components/CameraComponent.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

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

    void mouseButtonCallback(GLFWwindow *window, int button, int action, int)
    {
        auto &inputManager = CEngine::Engine::GetInstance().GetInputManager();
        if (action == GLFW_PRESS)
        {
            inputManager.SetMouseButtonPressed(button, true);
        }
        else if (action == GLFW_RELEASE)
        {
            inputManager.SetMouseButtonPressed(button, false);
        }
    }

    double lastx = 0;
    double lasty = 0;
    void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
    {
        double dx = xpos - lastx;
        double dy = ypos - lasty;

        auto now = std::chrono::system_clock::now();
        std::time_t c_time = std::chrono::system_clock::to_time_t(now);
        std::tm tm_time = *std::localtime(&c_time);

        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;

        auto &inputManager = CEngine::Engine::GetInstance().GetInputManager();
        inputManager.SetMousePositionOld(inputManager.GetMousePositionCurrent());
        glm::vec2 currentPos(static_cast<float>(xpos), static_cast<float>(ypos));
        inputManager.SetMousePositionCurrent(currentPos);
        inputManager.SetMousePositionChanged(true);

        Debug::Log("[dx: " + std::to_string(dx) + ", dy: " + std::to_string(dy) + "] | [xpos: " + std::to_string(xpos) + ", ypos: " + std::to_string(ypos) + "] - Time: " + std::to_string(millis));

        lastx = xpos;
        lasty = ypos;
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
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        // glfwSetCursorPosCallback(m_window, cursorPositionCallback);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glfwRawMouseMotionSupported())
        {
            glfwSetInputMode(m_window,GLFW_RAW_MOUSE_MOTION,GLFW_TRUE);
        }

        // Show Window
        glfwMakeContextCurrent(m_window);

        if (glewInit() != GLEW_OK)
        {
            Debug::LogError("GLEW is not ready");
            glfwTerminate();
            return false;
        }

        m_graphicsAPI.Init();
        m_physicsManager.Init();
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
#elif defined(_WIN32)
        m_lastTimePoint == std::chrono::high_resolution_clock::now();
#endif
        while(!glfwWindowShouldClose(m_window) && !m_application -> NeedToBeClosed())
        {
            glfwPollEvents();

#if defined(__linux__)
            auto now = std::chrono::steady_clock::now();
#elif defined(_WIN32)
            auto now = std::chrono::high_resolution_clock::now();
#endif
            float deltaTime = std::chrono::duration<float>(now - m_lastTimePoint).count();
            m_lastTimePoint = now;

            double xpos, ypos;
            glfwGetCursorPos(m_window, &xpos, &ypos);
            m_inputManager.MouseTracing(xpos, ypos);

            m_physicsManager.Update(deltaTime);

            m_application->Update(deltaTime);

            m_graphicsAPI.SetClearColor(1, 1, 1, 1);
            m_graphicsAPI.ClearBuffers();

            CameraData cameraData;
            std::vector<LightData> lights;

            int width = 0;
            int height = 0;
            glfwGetWindowSize(m_window, &width, &height);
            float aspect = static_cast<float>(width) / static_cast<float>(height);

            if (m_currentScene)
            {
                if (auto cameraObject = m_currentScene->GetMainCamera())
                {
                    // logic for matrices
                    auto cameraComponent = cameraObject->GetComponent<CameraComponent>();
                    if (cameraComponent)
                    {
                        cameraData.viewMatrix = cameraComponent->GetViewMatrix();
                        cameraData.projectionMatrix = cameraComponent->GetProjectionMatrix(aspect);
                        cameraData.position = cameraObject->GetWorldPosition();
                    }
                }

                lights = m_currentScene->CollectLights();
            }

            m_renderQueue.Draw(m_graphicsAPI, cameraData, lights);

            glfwSwapBuffers(m_window);
            // m_inputManager.SetMousePositionOld(m_inputManager.GetMousePositionCurrent());
            m_inputManager.SetMousePositionChanged(false);
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

    TextureManager &Engine::GetTextureManager()
    {
        return m_textureManager;
    }

    PhysicsManager &Engine::GetPhysicsManager()
    {
        return m_physicsManager;
    }

    GraphicsAPI &Engine::GetGraphicsAPI()
    {
        return m_graphicsAPI;
    }

    RenderQueue &Engine::GetRenderQueue()
    {
        return m_renderQueue;
    }

    FileSystem &Engine::GetFileSystem()
    {
        return m_fileSystem;
    }

    void Engine::SetScene(Scene *scene)
    {
        m_currentScene.reset(scene);
    }

    Scene *Engine::GetScene()
    {
        return m_currentScene.get();
    }
}
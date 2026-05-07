#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utils/Debug.h"

int main()
{
    // Check is glfw is initialized
    if (!glfwInit())
    {
        return -1;
    }

    // Define OpenGL and GLFW content version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Define GLFWwindow size and create
    GLFWwindow* window = glfwCreateWindow(1280, 720, "CEngine", nullptr, nullptr);
    Debug::Log("Start Create Window");
    
    if(window == nullptr)
    {
        Debug::LogError("Error creating window");
        glfwTerminate();
        return -1;
    }

    //Set window position
    glfwSetWindowPos(window, 2000, 150);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        Debug::LogError("GLEW is not ready");
        glfwTerminate();
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        // Draw Color
        glClearColor(1.0f, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap Buffer from back to front for display
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
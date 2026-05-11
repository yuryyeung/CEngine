#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utils/Debug.h"
#include "math/Vector.h"
#include <vector>

int main()
{
    // Check is glfw is initialized
    if (!glfwInit())
    {
        Debug::LogError("GLFW NOT BEEN INITIALIZED");
        return -1;
    }

    // Define OpenGL and GLFW content version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Define GLFWwindow size and create
    GLFWwindow *window = glfwCreateWindow(1280, 720, "CEngine", nullptr, nullptr);
    Debug::Log("Start Create Window");

    if (window == nullptr)
    {
        Debug::LogError("Error creating window");
        glfwTerminate();
        return -1;
    }

    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;

        void main()
        {
            gl_Position = vec4(position.x, position.y, position.z, 1.0);
        }
    )";

    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexShaderCStr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        Debug::LogError("ERROR:VERTEX_SHADER_COMPILATION_FAILED");
        Debug::LogError(infoLog);
    }

    // Fragment Shader
    std::string fragmentShaderSource = R"(
        #version 330
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentShaderSourceCStr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        Debug::LogError("ERROR:FRAGMENT_SHADER_COMPILATION_FAILED");
        Debug::LogError(infoLog);
    }

    // Shader Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        Debug::LogError("ERROR:SHADER_PROGRAM_LINKING_FAILED");
        Debug::LogError(infoLog);
    }

    // After linking, the shader is already apply to the Shader Program
    // Therefore you can delete the vertex and fragment shader for GIGO
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::vector<Vector3> vertices =
        {
            Vector3(0.0f, 0.5f, 0.0f),
            Vector3(-0.5f, -0.5f, 0.0f),
            Vector3(0.5f, -0.5f, 0.0f)};

    // Create Buffer Data
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set window position
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

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // Swap Buffer from back to front for display
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
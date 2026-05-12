#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "utils/Debug.h"
#include "math/Vector.h"
#include <vector>

Vector2 offset;
void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            Debug::Log("GLFW_KEY_UP");
            offset.y += 0.01f;
            break;
        case GLFW_KEY_DOWN:
            Debug::Log("GLFW_KEY_DOWN");
            offset.y -= 0.01f;
            break;
        case GLFW_KEY_LEFT:
            Debug::Log("GLFW_KEY_LEFT");
            offset.x -= 0.01f;
            break;
        case GLFW_KEY_RIGHT:
            Debug::Log("GLFW_KEY_RIGHT");
            offset.x += 0.01f;
            break;
        default:
            break;
        }
    }
}

int main()
{
#if defined(__linux__)
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
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

    glfwSetKeyCallback(window, keyCallback);

    // Set window position
    // glfwSetWindowPos(window, 2000, 150);

    // Show Window
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        Debug::LogError("GLEW is not ready");
        glfwTerminate();
        return -1;
    }

    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        uniform vec2 uOffset;

        out vec3 vColor;

        void main()
        {
            vColor = color;
            gl_Position = vec4(position.x + uOffset.x, position.y + uOffset.y, position.z, 1.0);
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
        #version 330 core
        out vec4 FragColor;

        in vec3 vColor;
        uniform vec4 uColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0) * uColor;
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
        Vector3(0.5f, 0.5f, 0.0f),
        Vector3(-0.5f, 0.5f, 0.0f),
        Vector3(-0.5f, -0.5f, 0.0f),
        Vector3(0.5f, -0.5f, 0.0f)
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3
    };

    std::vector<Vector3> colors =
    {
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 1.0f, 1.0f)
    };

    // Create Buffer Data
    GLuint vbo_pos;
    glGenBuffers(1, &vbo_pos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vbo_col;
    glGenBuffers(1, &vbo_col);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vector3), colors.data(), GL_STATIC_DRAW);

    Debug::Log("vertices Size: " + std::to_string(vertices.size()) + " | sizeof(Vector3): " + std::to_string(sizeof(Vector3)));

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vector3), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vector3), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLint uColorLoc = glGetUniformLocation(shaderProgram, "uColor");
    GLint uOffsetLoc = glGetUniformLocation(shaderProgram, "uOffset");

    while (!glfwWindowShouldClose(window))
    {
        // Draw Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform4f(uColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
        glUniform2f(uOffsetLoc, offset.x, offset.y);

        glBindVertexArray(vao);
        // glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        // Swap Buffer from back to front for display
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
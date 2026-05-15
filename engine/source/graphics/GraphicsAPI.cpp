#include "graphics/GraphicsAPI.h"
#include "graphics/ShaderProgram.h"
#include "utils/Debug.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

namespace CEngine
{
    std::shared_ptr<ShaderProgram> GraphicsAPI::CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
    {
        // Vertex Shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vertexShaderCStr = vertexSource.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr);
        glCompileShader(vertexShader);

        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            Debug::LogError("ERROR:VERTEX_SHADER_COMPILATION_FAILED" + std::string(infoLog));
            return nullptr;
        }

        // Fragment Shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fragmentShaderSourceCStr = fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            Debug::LogError("ERROR:FRAGMENT_SHADER_COMPILATION_FAILED: " + std::string(infoLog));
            return nullptr;
        }

        // Shader Program
        GLuint shaderProgramID = glCreateProgram();
        glAttachShader(shaderProgramID, vertexShader);
        glAttachShader(shaderProgramID, fragmentShader);
        glLinkProgram(shaderProgramID);

        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
            Debug::LogError("ERROR:SHADER_PROGRAM_LINKING_FAILED" + std::string(infoLog));
            return nullptr;
        }

        // After linking, the shader is already apply to the Shader Program
        // Therefore you can delete the vertex and fragment shader for GIGO
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return std::make_shared<ShaderProgram>(shaderProgramID);
    }

    void GraphicsAPI::BindShaderProgram(ShaderProgram* shaderProgram)
    {
        shaderProgram->Bind();
    }
}
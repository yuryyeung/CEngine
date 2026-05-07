#pragma once
#include <iostream>  
#include <string>
using namespace std;

class Debug
{
    public: 
    // Static method: call as Debug::Log("message")
    static void Log(const std::string& message);
    
    // Support for numbers/other types
    template <typename T>
    static void Log(T message) {
        std::cout << "[Log] " << message << std::endl;
    }

    static void LogWarning(const std::string& message);
    static void LogError(const std::string& message);
};
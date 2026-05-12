#include "Debug.h"

void Debug::Log(const std::string& message) {
    std::cout << "[Log] " << message << std::endl;
}

void Debug::LogWarning(const std::string& message) {
    std::cout << "\033[33m[Warning] " << message << "\033[0m" << std::endl;
}

void Debug::LogError(const std::string& message) {
    std::cout << "\033[31m[Error] " << message << "\033[0m" << std::endl;
}
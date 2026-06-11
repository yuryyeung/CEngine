#pragma once
#include <filesystem>

namespace CEngine
{
    class FileSystem
    {
        public: 
            std::filesystem::path GetExecutableFolder() const;
            std::filesystem::path GetAssetsFolder() const;
    };
}
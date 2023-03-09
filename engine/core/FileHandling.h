#pragma once
#include <filesystem>

namespace nimo
{
    class FileHandling{
    public:
        static bool Copy(const std::filesystem::path& from, const std::filesystem::path& to);
        static bool CreateDiretory(const std::filesystem::path& folderpath);
        static bool Exists(const std::filesystem::path& filepath);
    };
} // namespace nimo

#include "FileHandling.h"

#include <fstream>
#include "Log.h"

bool nimo::FileHandling::Copy(const std::filesystem::path& from, const std::filesystem::path& to)
{
    NIMO_DEBUG("Copying {} to {}", from.string(), to.string());
    if (FileHandling::Exists(to))
        return false;
    std::filesystem::copy(from, to, 
        std::filesystem::copy_options::recursive |
        std::filesystem::copy_options::update_existing
    );
    return true;
}

bool nimo::FileHandling::Move(const std::filesystem::path& from, const std::filesystem::path& to)
{
    if (nimo::FileHandling::Exists(to))
        return false;

    std::filesystem::rename(from, to);
    return true;
}

bool nimo::FileHandling::CreateDiretory(const std::filesystem::path& folderpath)
{
    NIMO_DEBUG("Creating directory {}", folderpath.string());
    return std::filesystem::create_directories(folderpath);
}

bool nimo::FileHandling::Exists(const std::filesystem::path& filepath)
{
    return std::filesystem::exists(filepath);
}
bool nimo::FileHandling::IsDirectory(const std::filesystem::path& filepath)
{
    return std::filesystem::is_directory(filepath);
}
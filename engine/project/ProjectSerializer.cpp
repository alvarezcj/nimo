#include "ProjectSerializer.h"
#include "json.hpp"
#include <fstream>

nimo::ProjectSerializer::ProjectSerializer(const std::shared_ptr<Project>& project)
    : m_project(project)
{}

void nimo::ProjectSerializer::Serialize(const std::filesystem::path& filepath)
{
    nlohmann::ordered_json j;
    j["Name"] = m_project->settings.name;
    j["AssetsDirectory"] = m_project->settings.assetDirectory;
    j["IndexPath"] = m_project->settings.assetIndexPath;
    j["LogsDirectory"] = m_project->settings.logsDirectory;
    j["StartScene"] = m_project->settings.startScene;
    std::ofstream ofs(filepath);
    ofs << j;
}
bool nimo::ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
{
    std::ifstream ifs(filepath);
    nlohmann::ordered_json j;
    j << ifs;
    m_project->settings.name = j["Name"];
    m_project->settings.assetDirectory = j["AssetsDirectory"];
    m_project->settings.assetIndexPath = j["IndexPath"];
    m_project->settings.logsDirectory = j["LogsDirectory"];
    m_project->settings.startScene = j["StartScene"];
    std::filesystem::path copypath = filepath;
    m_project->settings.projectDirectory = copypath.remove_filename();
    return true;
}
#pragma once 
#include <string>
#include <memory>
#include <filesystem>

#include "assets/AssetManager.h"
#include "scripting/ScriptManager.h"
#include "scene/SceneManager.h"

namespace nimo
{
    class ProjectSerializer;
    struct ProjectSettings
    {
        std::string name;
        std::string assetDirectory;
        std::string assetIndexPath;
        std::string logsDirectory;
        std::string modulesDirectory;
        std::string startScene;

        std::filesystem::path projectDirectory;
    };

    class Project
    {
    public:
        Project(const ProjectSettings& settings);
        ~Project();
        inline static void SetActiveProject(std::shared_ptr<Project> p ) 
        { 
            if(activeProject){
                SceneManager::SetActiveScene({});
                AssetManager::Cleanup();
                ScriptManager::Cleanup();
            }
            activeProject = p;
            if(activeProject){
                ScriptManager::Initialize();
                AssetManager::Initialize();
            }
        }
        inline static std::shared_ptr<Project> GetActiveProject(){ return activeProject;}
        inline ProjectSettings& GetSettings(){return settings;}
        inline static const std::filesystem::path GetAssetsFolderPath(){return GetActiveProject()->GetSettings().projectDirectory/GetActiveProject()->GetSettings().assetDirectory;}
        inline static const std::filesystem::path GetLogsFolderPath(){return GetActiveProject()->GetSettings().projectDirectory/GetActiveProject()->GetSettings().logsDirectory;}
        inline static const std::filesystem::path GetModulesFolderPath(){return GetActiveProject()->GetSettings().projectDirectory/GetActiveProject()->GetSettings().modulesDirectory;}
        inline static const std::filesystem::path GetAssetIndexPath(){return GetActiveProject()->GetSettings().projectDirectory/GetActiveProject()->GetSettings().assetIndexPath;}
    private:
        ProjectSettings settings;
        inline static std::shared_ptr<Project> activeProject;

        friend class ProjectSerializer;
    };
};
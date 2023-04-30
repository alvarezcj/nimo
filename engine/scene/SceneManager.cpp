#include "SceneManager.h"
#include "assets/AssetManager.h"
#include "scene/SceneSerializer.h"

std::shared_ptr<nimo::Scene> nimo::SceneManager::activeScene = {};

std::shared_ptr<nimo::Scene> nimo::SceneManager::CreateScene(const std::string& name)
{
    return std::make_shared<nimo::Scene>(name);
}

void nimo::SceneManager::LoadScene(const std::string& name)
{
    auto allScenes = AssetManager::GetAllExisting<Scene>();
    for(auto scene : allScenes)
    {
        if(scene.filepath.stem() == name)
        {
            // Name matches filename without extension
            LoadScene(scene.id);
        }
    }
}
void nimo::SceneManager::LoadScene(GUID id)
{
    auto loadedScene = AssetManager::Get<Scene>(id);
    SetActiveScene(loadedScene);
    AssetManager::UnloadUnused();
}
#pragma once
#include "Scene.h"

namespace nimo
{
class SceneManager
{
public:
    static std::shared_ptr<Scene> CreateScene(const std::string& name);
    static void LoadScene(const std::string& name);
    static void LoadScene(GUID id);
    inline static std::shared_ptr<Scene> GetActiveScene() {return activeScene;}
    inline static void SetActiveScene(std::shared_ptr<Scene> scene) {activeScene = scene;}
    inline static void AddLoadRequest(const std::string& name){loadSceneRequests.push_back(name);}
    inline static void ProcessLoadRequests() { 
        if(loadSceneRequests.size() == 0) return;
        for(auto name : loadSceneRequests) 
            LoadScene(name);
        loadSceneRequests.clear();
    }
private:
    static std::shared_ptr<Scene> activeScene;
    static std::vector<std::string> loadSceneRequests;
};
};
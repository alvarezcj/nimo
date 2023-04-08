#include "RuntimeLayer.h"
#include "project/ProjectSerializer.h"
#include "renderer/Renderer.h"
#include "scene/SceneSerializer.h"

void RuntimeLayer::OnAttach()
{
    nimo::ProjectSettings settings;
    auto project = std::make_shared<nimo::Project>(settings);
    nimo::ProjectSerializer ps(project);
    ps.Deserialize(projectFilepath);
    nimo::Project::SetActiveProject(project);
    nimo::AssetId startingSceneId = nimo::AssetId(nimo::Project::GetActiveProject()->GetSettings().startScene);
    NIMO_DEBUG("Loading scene {}", nimo::Project::GetActiveProject()->GetSettings().startScene);
    nimo::AssetManager::Get<nimo::Scene>(startingSceneId);
    renderer = std::make_shared<nimo::SceneRenderer>();
}
  
void RuntimeLayer::OnUpdate(float deltaTime)
{
    nimo::Renderer::BeginFrame();
    for(auto scene : nimo::AssetManager::GetAllLoaded<nimo::Scene>())
    {
        renderer->SetScene(scene);
        renderer->Render();
    }
    nimo::Renderer::EndFrame();
}
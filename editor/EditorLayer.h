#include "core/Log.h"
#include "core/Layer.h"
#include "core/FileHandling.h"
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/VertexArray.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "renderer/FrameBuffer.h"
#include "renderer/Mesh.h"
#include "assets/AssetManager.h"
#include "scene/Scene.h"
#include "scene/SceneSerializer.h"
#include "project/ProjectSerializer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "nfd.h"

class LogPanel;
class StatisticsPanel;
class GameViewPanel;
class SceneContentsPanel;
class InspectorPanel;
class AssetExplorerPanel;
class SceneViewPanel;

class EditorLayer : public nimo::Layer
{
public:
    EditorLayer(){
        assetIcons[nimo::AssetType::None] = std::make_shared<nimo::Texture>("icon_file.png");
        assetIcons[nimo::AssetType::Texture] = std::make_shared<nimo::Texture>("icon_camera.png");
        assetIcons[nimo::AssetType::Mesh] = std::make_shared<nimo::Texture>("icon_cube.png");
        assetIcons[nimo::AssetType::Shader] = std::make_shared<nimo::Texture>("icon_polaroid.png");
        assetIcons[nimo::AssetType::Material] = std::make_shared<nimo::Texture>("icon_paintroll.png");
        assetIcons[nimo::AssetType::Scene] = std::make_shared<nimo::Texture>("icon_castle.png");
    }
    ~EditorLayer()
    {
        delete logPanel;
        delete statisticsPanel;
        delete gameViewPanel;
        delete inspectorPanel;
        delete sceneContentsPanel;
        delete assetExplorerPanel;
        delete sceneViewPanel;
    }
private:
    friend class GameViewPanel;
    friend class SceneContentsPanel;
    friend class InspectorPanel;
    friend class AssetExplorerPanel;
    friend class SceneViewPanel;

    nimo::FrameBuffer::Details d;
    std::shared_ptr<nimo::FrameBuffer> fb;
    LogPanel* logPanel;
    StatisticsPanel* statisticsPanel;
    GameViewPanel* gameViewPanel;
    SceneViewPanel* sceneViewPanel;
    InspectorPanel* inspectorPanel;
    SceneContentsPanel* sceneContentsPanel;
    AssetExplorerPanel* assetExplorerPanel;
    nimo::GUID lastModifiedScene;

    std::map<nimo::AssetType, std::shared_ptr<nimo::Texture>> assetIcons;

    void OnAttach() override;
    void OnUpdate(float deltaTime) override;

    void CreateNewProject(const std::filesystem::path& folder, const std::string& name);
    std::shared_ptr<nimo::Texture> entityIcon;
};
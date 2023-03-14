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



    void OnAttach() override;
    void OnUpdate(float deltaTime) override;

    void CreateNewProject(const std::filesystem::path& folder, const std::string& name);
    std::shared_ptr<nimo::Texture> entityIcon;
};
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
#include "assets/MeshSource.h"
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

class EditorLayer : public nimo::Layer
{
    friend class GameViewPanel;
    friend class SceneContentsPanel;

    nimo::FrameBuffer::Details d;
    std::shared_ptr<nimo::FrameBuffer> fb;
    LogPanel* logPanel;
    StatisticsPanel* statisticsPanel;
    GameViewPanel* gameViewPanel;
    SceneContentsPanel* sceneContentsPanel;
    std::map<std::string,std::pair<std::filesystem::path, std::shared_ptr<nimo::Scene>>> loadedScenes;
    std::string lastModifiedScene;

    void OnAttach() override;
    void OnUpdate() override;
};
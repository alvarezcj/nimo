#include "EditorLayer.h"

#include "LogPanel.h"
#include "StatisticsPanel.h"
#include "GameViewPanel.h"
#include "SceneViewPanel.h"
#include "SceneContentsPanel.h"
#include "InspectorPanel.h"
#include "AssetExplorerPanel.h"
#include "misc/cpp/imgui_stdlib.h"

#include "core/Application.h"

void EditorLayer::OnAttach()
{
    d.width = 1920;
    d.height = 1080;
    entityIcon = std::make_shared<nimo::Texture>("menuGrid.png");
    fb = std::make_shared<nimo::FrameBuffer>(d);
    logPanel = new LogPanel();
    statisticsPanel = new StatisticsPanel();
    gameViewPanel = new GameViewPanel(this);
    sceneViewPanel = new SceneViewPanel(this);
    sceneContentsPanel = new SceneContentsPanel(this);
    inspectorPanel = new InspectorPanel(this);
    assetExplorerPanel = new AssetExplorerPanel(this);

    auto& style = ImGui::GetStyle();
    auto& colors = ImGui::GetStyle().Colors;

    //========================================================
    /// Fonts
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF("fonts/Quicksand/static/Quicksand-Medium.ttf", 20.0f);

    //========================================================
    /// Colours

    // Headers
    colors[ImGuiCol_MenuBarBg]			= ImColor(16, 16, 16, 255);
    colors[ImGuiCol_Header]				= ImColor(188, 108, 37, 255);
    colors[ImGuiCol_HeaderHovered]		= ImColor(188, 108, 37, 125);
    colors[ImGuiCol_HeaderActive]		= ImColor(188, 108, 37, 255);

    // Buttons
    colors[ImGuiCol_Button]				= ImColor(188, 108, 37, 200);
    colors[ImGuiCol_ButtonHovered]		= ImColor(251, 209, 162, 150);
    colors[ImGuiCol_ButtonActive]		= ImColor(188, 108, 37, 150);

    // Frame BG
    colors[ImGuiCol_FrameBg]			= ImColor(16, 16, 16, 200);
    colors[ImGuiCol_FrameBgHovered]		= ImColor(66, 66, 66, 255);
    colors[ImGuiCol_FrameBgActive]		= ImColor(66, 66, 66, 255);

    // Tabs
    colors[ImGuiCol_Tab]				= ImColor(44, 44, 44, 255);
    colors[ImGuiCol_TabHovered]			= ImColor(188, 108, 37, 255);
    colors[ImGuiCol_TabActive]			= ImColor(188, 108, 37, 225);
    colors[ImGuiCol_TabUnfocused]		= ImColor(44, 44, 44, 255);
    colors[ImGuiCol_TabUnfocusedActive] = ImColor(188, 108, 37, 150);

    // Title
    colors[ImGuiCol_TitleBg]			= ImColor(16, 16, 16, 255);
    colors[ImGuiCol_TitleBgActive]		= ImColor(16, 16, 16, 255);
    colors[ImGuiCol_TitleBgCollapsed]	= ImColor(16, 16, 16, 255);

    // Resize Grip
    colors[ImGuiCol_ResizeGrip]			= ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]	= ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]	= ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg]		= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]		= ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

    // Check Mark
    colors[ImGuiCol_CheckMark]			= ImColor(200, 200, 200, 255);

    // Slider
    colors[ImGuiCol_SliderGrab]			= ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
    colors[ImGuiCol_SliderGrabActive]	= ImVec4(0.66f, 0.66f, 0.66f, 1.0f);

    // Text
    colors[ImGuiCol_Text]				= ImColor(240, 240, 240, 255);

    // Checkbox
    colors[ImGuiCol_CheckMark]			= ImColor(240, 240, 240, 255);

    // Separator
    colors[ImGuiCol_Separator]			= ImColor(16, 16, 16, 200);
    colors[ImGuiCol_SeparatorActive]	= ImColor(251, 209, 162, 255);
    colors[ImGuiCol_SeparatorHovered]	= ImColor(251, 209, 162, 150);

    // Window Background
    colors[ImGuiCol_WindowBg]			= ImColor(44, 44, 44, 255);
    colors[ImGuiCol_ChildBg]			= ImColor(66, 66, 66, 255);
    colors[ImGuiCol_PopupBg]			= ImColor(66, 66, 66, 255);
    colors[ImGuiCol_Border]				= ImColor(66, 66, 66, 255);

    colors[ImGuiCol_DragDropTarget] = ImColor(200,200,200, 255);

    // Tables
    // colors[ImGuiCol_TableHeaderBg]		= ImGui::ColorConvertU32ToFloat4(Colours::Theme::groupHeader);
    // colors[ImGuiCol_TableBorderLight]	= ImGui::ColorConvertU32ToFloat4(Colours::Theme::backgroundDark);

    // Menubar
    colors[ImGuiCol_MenuBarBg]			= ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };

    //========================================================
    /// Style
    style.FrameRounding = 10.0f;
    style.FrameBorderSize = 0.0f;
    style.IndentSpacing = 20.0f;
    style.WindowPadding = ImVec2(5.f,5.f);
    style.FramePadding = ImVec2(5.f,3.f);
    style.ItemSpacing = ImVec2(5.f,4.f);
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.GrabMinSize = 10.f;
    style.GrabRounding = 3.f;
}
    void EditorLayer::OnUpdate(float deltaTime) 
    {
    
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow(NULL);

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoWindowMenuButton;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);


        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("DockSpace");
            ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        ImGui::End();

        static bool showCreateNewProjectModal = false;

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene"))
                {
                    if(nimo::Project::GetActiveProject())
                    {
                        // Unload current scene
                        // Create new Scene
                        // Show new scene
                    }
                    else
                    {
                        NIMO_ERROR("No project loaded to create new scene");
                    }
                }
                if (ImGui::MenuItem("Open Scene"))
                {
                    if(nimo::Project::GetActiveProject())
                    {
                        nfdchar_t *outPath = NULL;
                        nfdresult_t result = NFD_OpenDialog( "nscene", NULL, &outPath );
                            
                        if ( result == NFD_OKAY ) {
                            std::filesystem::path scenePath(outPath);
                            size_t lastdot = scenePath.filename().string().find_last_of(".");
                            std::string sceneName;
                            if (lastdot == std::string::npos)
                                sceneName = scenePath.filename().string();
                            else
                                sceneName = scenePath.filename().string().substr(0, lastdot);
                            
                            std::shared_ptr<nimo::Scene> scene = std::make_shared<nimo::Scene>();
                            scene->SetName(sceneName);
                            if(!scene->id.valid())
                            {
                                scene->id = nimo::GUID::Create();
                            }
                            free(outPath);
                        }
                    }
                    else
                    {
                        NIMO_ERROR("No project loaded to create new scene");
                    }
                }
                ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
                if (ImGui::MenuItem("Save"))
                {
                    if(lastModifiedScene.valid())
                    {
                        nimo::AssetManager::Serialize<nimo::Scene>(lastModifiedScene);
                    }
                }
                ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
                if (ImGui::MenuItem("New Project..."))
                {
                    showCreateNewProjectModal = true;
                }
                if (ImGui::MenuItem("Open Project..."))
                {
                    nfdchar_t *outPath = NULL;
                    nfdresult_t result = NFD_OpenDialog( "nproj", NULL, &outPath );
                        
                    if ( result == NFD_OKAY ) {
                        nimo::ProjectSettings settings;
                        std::shared_ptr<nimo::Project> project = std::make_shared<nimo::Project>(settings);
                        nimo::ProjectSerializer projectSer(project);
                        projectSer.Deserialize(outPath);
                        nimo::Project::SetActiveProject(project);
                        nimo::AssetId startingSceneId = nimo::AssetId(nimo::Project::GetActiveProject()->GetSettings().startScene);
                        NIMO_DEBUG(nimo::AssetManager::GetMetadata(startingSceneId).filepath.string());
                        nimo::AssetManager::Get<nimo::Scene>(startingSceneId);
                        lastModifiedScene = startingSceneId;
                        free(outPath);
                    }
                }
                if (ImGui::MenuItem("Save Project"))
                {
                    nimo::ProjectSerializer projectSer(nimo::Project::GetActiveProject());
                    projectSer.Serialize((nimo::Project::GetActiveProject()->GetSettings().projectDirectory/nimo::Project::GetActiveProject()->GetSettings().name).replace_extension(".nproj"));
                }
                ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
                if (ImGui::MenuItem("Exit"))
                {
                    nimo::Application::Instance().Close();
                }
                ImGui::EndMenu(); 
            }
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Scene", NULL, &sceneContentsPanel->open);
                ImGui::MenuItem("Game", NULL, &gameViewPanel->open);
                ImGui::MenuItem("Inspector", NULL, &inspectorPanel->open);
                ImGui::MenuItem("Log", NULL, &logPanel->open);
                ImGui::MenuItem("Statistics", NULL, &statisticsPanel->open);
                ImGui::EndMenu(); 
            }
            // Always center this window when appearing
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        }
        ImGui::EndMainMenuBar();

        if(showCreateNewProjectModal)
            ImGui::OpenPopup("Create New Project");

        static std::string projectName = "";
        static std::string projectLocation = "";
        if (ImGui::BeginPopupModal("Create New Project", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            showCreateNewProjectModal = false;
            ImGui::Text("PROJECT SETTINGS\n");
            ImGui::Separator();

            ImGui::InputTextWithHint("##CreateProject##Name", "Project Name", &projectName);
            ImGui::InputTextWithHint("##CreateProject##Location", "Project Location", &projectLocation);
            ImGui::SameLine();
            if (ImGui::Button("...", ImVec2(50, 0))) { 
                nfdchar_t *folderPath = NULL;
                nfdresult_t result = NFD_PickFolder(NULL, &folderPath);
                if ( result == NFD_OKAY )
                {
                    projectLocation = folderPath;
                    free(folderPath);
                }
            }

            if (ImGui::Button("OK", ImVec2(120, 0))) 
            { 
                CreateNewProject(projectLocation, projectName);
                projectName = "";
                ImGui::CloseCurrentPopup(); 
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) 
            { 
                projectName = "";
                projectLocation = "";
                ImGui::CloseCurrentPopup(); 
            }
            ImGui::EndPopup();
        }
        
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        gameViewPanel->Render();
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        sceneViewPanel->Render();

        ImGui::SetNextWindowSize(ImVec2(600, 100), ImGuiCond_FirstUseEver);
        statisticsPanel->Render();

        ImGui::SetNextWindowSize(ImVec2(600, 100), ImGuiCond_FirstUseEver);
        logPanel->Render();

        ImGui::SetNextWindowSize(ImVec2(400, 800), ImGuiCond_FirstUseEver);
        sceneContentsPanel->Render();

        ImGui::SetNextWindowSize(ImVec2(400, 800), ImGuiCond_FirstUseEver);
        inspectorPanel->Render();

        ImGui::SetNextWindowSize(ImVec2(1000, 200), ImGuiCond_FirstUseEver);
        assetExplorerPanel->Render();

        ImGui::Render();

        nimo::Renderer::BeginFrame(fb);
        for(auto scene : nimo::AssetManager::GetAllLoaded<nimo::Scene>())
        {
            scene->Update();
        }
        nimo::Renderer::EndFrame();

        nimo::Renderer::BeginFrame();        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        nimo::Renderer::EndFrame();
    }


void EditorLayer::CreateNewProject(const std::filesystem::path& folder, const std::string& name)
{
    if(!(nimo::FileHandling::Exists(folder) && nimo::FileHandling::IsDirectory(folder))) 
    {
        NIMO_ERROR("Path specified for project creation does not exits or it's not a folder: {}", folder.string());
        return;
    }
    auto projectFolderPath = folder/name;
    if(nimo::FileHandling::CreateDiretory(projectFolderPath))
    {
        NIMO_DEBUG("Created directory {}", projectFolderPath.string());
        nimo::ProjectSettings settings;
        settings.name = projectFolderPath.filename().string();
        settings.projectDirectory = projectFolderPath;
        settings.assetDirectory = "Assets";
        settings.assetIndexPath = name + ".nidx";
        settings.logsDirectory = "Logs";
        settings.startScene = "NewScene";
        std::shared_ptr<nimo::Project> project = std::make_shared<nimo::Project>(settings);
        nimo::Project::SetActiveProject(project);

        nimo::FileHandling::CreateDiretory(projectFolderPath/"Assets"/"Scenes");
        nimo::FileHandling::CreateDiretory(projectFolderPath/"Logs");
        nimo::FileHandling::Copy("shaders", projectFolderPath/"Assets"/"Shaders");
        nimo::FileHandling::Copy("objects", projectFolderPath/"Assets"/"Objects");
        nimo::FileHandling::Copy("materials", projectFolderPath/"Assets"/"Materials");
        nimo::AssetManager::CreateNewAsset<nimo::Shader>("Shaders/unlit_color.nshader");
        nimo::AssetManager::CreateNewAsset<nimo::Shader>("Shaders/unlit_texture.nshader");
        nimo::AssetManager::CreateNewAsset<nimo::MeshSource>("Objects/cube/cube.obj");
        nimo::AssetManager::CreateNewAsset<nimo::Texture>("Objects/cube/cube.png");

        auto unlitShader = nimo::AssetManager::Get<nimo::Shader>("Shaders/unlit_texture.nshader");
        std::vector<nimo::IMaterialProperty*> props;
        {
            auto prop = new nimo::MaterialPropertyTexture();
            prop->name = "mainTexture";
            prop->type = nimo::MaterialPropertyType::Texture;
            prop->SetTexture(nimo::AssetManager::Get<nimo::Texture>("Objects/cube/cube.png"));
            prop->SetValue(0);
            props.push_back(prop);
        }
        {
            auto prop = new nimo::MaterialProperty<glm::vec2>();
            prop->name = "tiling";
            prop->type = nimo::MaterialPropertyType::Vector2;
            prop->SetValue({1.0f, 1.0f});
            props.push_back(prop);
        }
        {
            auto prop = new nimo::MaterialProperty<glm::vec2>();
            prop->name = "offset";
            prop->type = nimo::MaterialPropertyType::Vector2;
            prop->SetValue({0.0f, 0.0f});
            props.push_back(prop);
        }
        std::shared_ptr<nimo::Material> material = std::make_shared<nimo::Material>(unlitShader, props);
        nimo::AssetManager::CreateAssetFromMemory<nimo::Material>("Materials/test.nmat", material);

        std::shared_ptr<nimo::Scene> createdScene = std::make_shared<nimo::Scene>();
        {
            auto e = createdScene->CreateEntity("MainCamera");
            e.GetComponent<nimo::TransformComponent>().Translation = {0.0f, 0.0f, -5.0f};
            e.AddComponent<nimo::CameraComponent>();
        }
        {
            nimo::Entity ent_cube1 = createdScene->CreateEntity("Cube");
            ent_cube1.GetComponent<nimo::TransformComponent>().Translation = {0.0f, 0.0f, 0.0f};
            ent_cube1.AddComponent<nimo::MeshComponent>().source = nimo::AssetManager::Get<nimo::MeshSource>("Objects/cube/cube.obj");
            ent_cube1.AddComponent<nimo::MeshRendererComponent>().material = nimo::AssetManager::Get<nimo::Material>("Materials/test.nmat");
            ent_cube1.GetComponent<nimo::MeshRendererComponent>().mesh = std::make_shared<nimo::Mesh>(ent_cube1.GetComponent<nimo::MeshComponent>().source);
        }
        nimo::AssetManager::CreateAssetFromMemory<nimo::Scene>("Scenes/NewScene.nscene", createdScene);
        nimo::AssetManager::WriteIndex();

        nimo::Project::GetActiveProject()->GetSettings().startScene = nimo::AssetManager::Get<nimo::Scene>("Scenes/NewScene.nscene")->id.str();
        nimo::ProjectSerializer projectSer(project);
        projectSer.Serialize((projectFolderPath/settings.name).replace_extension(".nproj"));
        NIMO_DEBUG("Serialized Project {}", settings.name);
    }
    else{
        NIMO_ERROR("Error Creating directory {}", projectFolderPath.string());
    }
}

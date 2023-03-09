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

class EditorLayer : public nimo::Layer
{
    nimo::FrameBuffer::Details d;
    std::shared_ptr<nimo::FrameBuffer> fb;
    void OnAttach() override{
        d.width = 1920;
        d.height = 1080;
        fb = std::make_shared<nimo::FrameBuffer>(d);

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
		colors[ImGuiCol_Header]				= ImColor(247, 146, 86, 255);
		colors[ImGuiCol_HeaderHovered]		= ImColor(247, 146, 86, 125);
		colors[ImGuiCol_HeaderActive]		= ImColor(247, 146, 86, 255);

		// Buttons
		colors[ImGuiCol_Button]				= ImColor(247, 146, 86, 200);
		colors[ImGuiCol_ButtonHovered]		= ImColor(251, 209, 162, 150);
		colors[ImGuiCol_ButtonActive]		= ImColor(247, 146, 86, 150);

		// Frame BG
		colors[ImGuiCol_FrameBg]			= ImColor(16, 16, 16, 200);
		colors[ImGuiCol_FrameBgHovered]		= ImColor(66, 66, 66, 255);
		colors[ImGuiCol_FrameBgActive]		= ImColor(66, 66, 66, 255);

		// Tabs
		colors[ImGuiCol_Tab]				= ImColor(44, 44, 44, 255);
		colors[ImGuiCol_TabHovered]			= ImColor(247, 146, 86, 255);
		colors[ImGuiCol_TabActive]			= ImColor(247, 146, 86, 225);
		colors[ImGuiCol_TabUnfocused]		= ImColor(44, 44, 44, 255);
		colors[ImGuiCol_TabUnfocusedActive] = ImColor(247, 146, 86, 150);

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
        style.ItemSpacing = ImVec2(5.f,3.f);
        style.WindowBorderSize = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.GrabMinSize = 10.f;
        style.GrabRounding = 3.f;
    }
    void OnUpdate() override
    {
        static std::map<std::string,std::pair<std::filesystem::path, std::shared_ptr<nimo::Scene>>> loadedScenes;
        static std::string lastModifiedScene;
    
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(NULL);

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
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
        
        ImGui::Begin("Game");
        {
            // Using a Child allow to fill all the space of the window.
            // It also alows customization
            ImGui::BeginChild("GameRender");
            // Get the size of the child (i.e. the whole draw size of the windows).
            ImVec2 wsize = ImGui::GetWindowSize();
            // Because I use the texture from OpenGL, I need to invert the V from the UV.
            ImGui::Image((ImTextureID)fb->GetColorAttachmentId(), wsize, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::Begin("Statistics"); 
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

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
                            loadedScenes[sceneName] = std::make_pair(scenePath, scene);
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
                    if(!lastModifiedScene.empty())
                    {
                        auto info = loadedScenes[lastModifiedScene];
                        nimo::AssetManager::CreateAssetFromMemory<nimo::Scene>(info.first.string(), info.second);
                    }
                }
                ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
                if (ImGui::MenuItem("New Project..."))
                {
                    nfdchar_t *folderPath = NULL;
                    nfdresult_t result = NFD_PickFolder(NULL, &folderPath);
                    if ( result == NFD_OKAY )
                    {
                        std::filesystem::path folder(folderPath);
                        std::filesystem::path projectFolderPath = folder/"TestProject";
                        if(nimo::FileHandling::CreateDiretory(projectFolderPath))
                        {
                            NIMO_DEBUG("Created directory {}", projectFolderPath.string());
                            nimo::ProjectSettings settings;
                            settings.name = projectFolderPath.filename().string();
                            settings.projectDirectory = projectFolderPath;
                            settings.assetDirectory = ("Assets");
                            settings.assetIndexPath = ("TestProject.nidx");
                            settings.logsDirectory = ("Logs");
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
                                e.AddComponent<nimo::TransformComponent>().Translation = {0.0f, 0.0f, -5.0f};
                                e.AddComponent<nimo::CameraComponent>();
                            }
                            {
                                nimo::Entity ent_cube1 = createdScene->CreateEntity("Cube");
                                ent_cube1.AddComponent<nimo::TransformComponent>().Translation = {0.0f, 0.0f, 0.0f};
                                ent_cube1.AddComponent<nimo::MeshComponent>().source = nimo::AssetManager::Get<nimo::MeshSource>("Objects/cube/cube.obj");
                                ent_cube1.AddComponent<nimo::MeshRendererComponent>().material = nimo::AssetManager::Get<nimo::Material>("Materials/test.nmat");
                                ent_cube1.GetComponent<nimo::MeshRendererComponent>().mesh = std::make_shared<nimo::Mesh>(ent_cube1.GetComponent<nimo::MeshComponent>().source);
                            }
                            nimo::AssetManager::CreateAssetFromMemory<nimo::Scene>("Scenes/NewScene.nscene", createdScene);
                            nimo::AssetManager::WriteIndex();
                            loadedScenes["NewScene"] = std::make_pair(projectFolderPath/"Assets"/"Scenes"/"NewScene.nscene", createdScene);

                            nimo::Project::GetActiveProject()->GetSettings().startScene = nimo::AssetManager::Get<nimo::Scene>("Scenes/NewScene.nscene")->id.str();
                            nimo::ProjectSerializer projectSer(project);
                            projectSer.Serialize((projectFolderPath/settings.name).replace_extension(".nproj"));
                            NIMO_DEBUG("Serialized Project {}", settings.name);
                        }
                        else{
                            NIMO_ERROR("Creating directory {}", projectFolderPath.string());
                        }
                        free(folderPath);
                    }
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
                        loadedScenes[nimo::AssetManager::GetMetadata(startingSceneId).filepath.filename().string()] = std::make_pair(nimo::AssetManager::GetMetadata(startingSceneId).filepath, nimo::AssetManager::Get<nimo::Scene>(startingSceneId));                        ;
                        lastModifiedScene = nimo::AssetManager::GetMetadata(startingSceneId).filepath.filename().string();
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
                    
                }
                ImGui::EndMenu(); 
            }
        }
        ImGui::EndMainMenuBar();


        ImGui::Begin("Scene"); 
        
        for(auto [name, scene] : loadedScenes)
        {
            float totalHeight = 0;
            ImVec2 rectMin;
            ImVec2 rectMax;
            bool res;
            if(ImGui::CollapsingHeader((name + "##" + scene.second->id.str()).c_str()))
            {
                rectMin = ImGui::GetItemRectMin();
                rectMax = ImGui::GetItemRectMax();
                totalHeight += ImGui::GetItemRectSize().y;
                scene.second->ForEachEntity([&](nimo::Entity& ent)
                {
                    std::string entityName =  ent.GetComponent<nimo::LabelComponent>().Label;
                    std::string entityIdString =  ent.GetComponent<nimo::IDComponent>().Id.str();
                    if (ImGui::TreeNode((entityName + "##" + scene.second->id.str()).c_str()))
                    {
                        totalHeight += ImGui::GetItemRectSize().y;
                        if(ent.HasComponent<nimo::TransformComponent>())
                        {
                            if (ImGui::TreeNode((std::string("Transform##")+entityName).c_str()))
                            {
                                totalHeight += ImGui::GetItemRectSize().y;
                                std::string p = "Position##";
                                p += entityIdString;
                                std::string r = "Rotation##";
                                r += entityIdString;
                                std::string s = "Scale##";
                                s += entityIdString;
                                ImGui::DragFloat3(p.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Translation, 0.1f);
                                totalHeight += ImGui::GetItemRectSize().y;
                                ImGui::DragFloat3(r.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Rotation, 0.1f);
                                totalHeight += ImGui::GetItemRectSize().y;
                                ImGui::DragFloat3(s.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Scale, 0.1f);
                                totalHeight += ImGui::GetItemRectSize().y;
                                ImGui::TreePop();
                            }
                            else{
                                totalHeight += ImGui::GetItemRectSize().y;
                            }
                        }
                        if(ent.HasComponent<nimo::CameraComponent>())
                        {
                            if (ImGui::TreeNode((std::string("Camera##")+entityName).c_str()))
                            {
                                totalHeight += ImGui::GetItemRectSize().y;
                                static int item_current_2 = 0;
                                ImGui::Combo((std::string("Projection##") + (std::string("Camera##")+entityName)).c_str(), &item_current_2, "Perspective\0Orthographic\0\0");
                                totalHeight += ImGui::GetItemRectSize().y;
                                if(item_current_2 == 0)
                                {
                                    ent.GetComponent<nimo::CameraComponent>().Projection = nimo::CameraComponent::Projection::Perspective;
                                    ImGui::SliderFloat((std::string("FOV##") + (std::string("Camera##")+entityName)).c_str(), &ent.GetComponent<nimo::CameraComponent>().FOV, 0.1f, 179.0f);
                                    totalHeight += ImGui::GetItemRectSize().y;
                                }
                                if(item_current_2 == 1)
                                {
                                    ent.GetComponent<nimo::CameraComponent>().Projection = nimo::CameraComponent::Projection::Orthographic;
                                }
                                ImGui::DragFloat((std::string("Clipping plane Far##") + (std::string("Camera##")+entityName)).c_str(), &ent.GetComponent<nimo::CameraComponent>().ClippingPlanes.Far, 0.1f, 0.0f);
                                totalHeight += ImGui::GetItemRectSize().y;
                                ImGui::DragFloat((std::string("Clipping plane Near##") + (std::string("Camera##")+entityName)).c_str(), &ent.GetComponent<nimo::CameraComponent>().ClippingPlanes.Near, 0.1f, 0.0f);
                                totalHeight += ImGui::GetItemRectSize().y;

                                ImGui::TreePop();
                            }
                            else{
                                totalHeight += ImGui::GetItemRectSize().y;
                            }
                        }
                        if(ent.HasComponent<nimo::MeshRendererComponent>())
                        {
                            if (ImGui::TreeNode((std::string("Mesh Renderer##")+entityName).c_str()))
                            {
                                totalHeight += ImGui::GetItemRectSize().y;
                                if (ImGui::TreeNode((std::string("Material##") + (std::string("Mesh##")+entityName)).c_str()))
                                {
                                    totalHeight += ImGui::GetItemRectSize().y;
                                    auto mat = ent.GetComponent<nimo::MeshRendererComponent>().material;
                                    for(auto p: mat->properties)
                                    {
                                        switch (p->type)
                                        {
                                        case nimo::MaterialPropertyType::Vector2:
                                            {
                                                ImGui::DragFloat2((p->name + "##" + (std::string("Material##")+ (std::string("Mesh Renderer##")+entityName))).c_str(), (float*)p->GetDataPtr(), 0.1f);
                                                totalHeight += ImGui::GetItemRectSize().y;
                                            }
                                            break;
                                        case nimo::MaterialPropertyType::Texture:
                                            {
                                                ImGui::InputInt((p->name + "##" + (std::string("Material##")+ (std::string("Mesh Renderer##")+entityName))).c_str(), (int*)p->GetDataPtr());
                                                totalHeight += ImGui::GetItemRectSize().y;
                                            }
                                            break;
                                        
                                        default:
                                            break;
                                        }
                                    }
                                    ImGui::TreePop();
                                }

                            else{
                                totalHeight += ImGui::GetItemRectSize().y;
                            }
                                ImGui::TreePop();
                            }
                            else{
                                totalHeight += ImGui::GetItemRectSize().y;
                            }
                        }
                        if(ent.HasComponent<nimo::MeshComponent>())
                        {
                            if (ImGui::TreeNode((std::string("Mesh##")+entityName).c_str()))
                            {
                                totalHeight += ImGui::GetItemRectSize().y;
                                ImGui::Text((std::string("Mesh asset:").c_str()));
                                ImGui::SameLine();
                                ImGui::Text((std::string(nimo::AssetManager::GetMetadata(ent.GetComponent<nimo::MeshComponent>().source->id).filepath.string())).c_str());
                                totalHeight += ImGui::GetItemRectSize().y;

                                ImGui::TreePop();
                            }
                            else{
                                totalHeight += ImGui::GetItemRectSize().y;
                            }
                        }
                        ImGui::TreePop();
                    }
                    else{
                        totalHeight += ImGui::GetItemRectSize().y;
                    }

                });
            }
            else{
                totalHeight += ImGui::GetItemRectSize().y;
            }
            res = ImGui::IsMouseHoveringRect(rectMin, ImVec2(rectMax.x, rectMax.y + totalHeight)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
            ImGui::Separator();
            if(res)
            {
                lastModifiedScene = name;
            }
            if(ImGui::Button("Add cube entity..."))
            {
                nimo::Entity ent_cube1 = scene.second->CreateEntity();
                ent_cube1.AddComponent<nimo::TransformComponent>().Translation = {1.0f, 1.0f, -3.0f};
                ent_cube1.AddComponent<nimo::MeshComponent>().source = nimo::AssetManager::Get<nimo::MeshSource>("Objects/cube/cube.obj");
                ent_cube1.AddComponent<nimo::MeshRendererComponent>().material = nimo::AssetManager::Get<nimo::Material>("Materials/test.nmat");
                ent_cube1.GetComponent<nimo::MeshRendererComponent>().mesh = std::make_shared<nimo::Mesh>(ent_cube1.GetComponent<nimo::MeshComponent>().source);
            }
        }
        ImGui::End();
        ImGui::Render();


        nimo::Renderer::BeginFrame(fb);
        for(auto [name, scene] : loadedScenes)
        {
            nimo::AssetManager::Get<nimo::Shader>("Shaders/unlit_texture.nshader")->use();
            scene.second->Update(*nimo::AssetManager::Get<nimo::Shader>("Shaders/unlit_texture.nshader"));
        }
        nimo::Renderer::EndFrame();

        nimo::Renderer::BeginFrame();
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        nimo::Renderer::EndFrame();
    }
};
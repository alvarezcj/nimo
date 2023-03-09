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
        d.width = 1280;
        d.height = 720;
        fb = std::make_shared<nimo::FrameBuffer>(d);
    }
    void OnUpdate() override
    {
        static std::map<std::string,std::pair<std::filesystem::path, std::shared_ptr<nimo::Scene>>> loadedScenes;
        static std::string lastModifiedScene;
    
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow(&show_demo_window);

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

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
        static auto first_time = true;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            // ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);

            if (first_time)
            {
                first_time = false;

                ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

                // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
                //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
                // out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
                ImGuiID dock_id_stats;
                ImGuiID dock_id_game;
                ImGuiID dock_id_menu;
                ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.1f, &dock_id_stats, &dock_id_game);
                ImGui::DockBuilderSplitNode(dock_id_game, ImGuiDir_Right, 0.8f, &dock_id_game, &dock_id_menu);
                // auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
                // auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 1.0f, nullptr, &dockspace_id);

                // we now dock our windows into the docking node we made above
                ImGui::DockBuilderDockWindow("Game", dock_id_game);
                ImGui::DockBuilderDockWindow("Statistics", dock_id_stats);
                ImGui::DockBuilderDockWindow("Scene", dock_id_menu);
                ImGui::DockBuilderFinish(dockspace_id);
            }
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
            if(scene.second->UpdateGui())
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
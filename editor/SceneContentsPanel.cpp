#include "SceneContentsPanel.h"
#include "imgui.h"
#include "EditorLayer.h"

void SceneContentsPanel::OnRender()
{
    for(auto [name, scene] : m_editor->loadedScenes)
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
            m_editor->lastModifiedScene = name;
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
}

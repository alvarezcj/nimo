#include "InspectorPanel.h"
#include "imgui.h"
#include "EditorLayer.h"
#include "core/Log.h"
#include "misc/cpp/imgui_stdlib.h"

void InspectorPanel::OnRender()
{
    if(!selectedItem.valid()) return;
    for(auto [name, scene] : m_editor->loadedScenes)
    {
        auto ent = scene.second->GetEntity(selectedItem);
        auto entityIdString = ent.GetComponent<nimo::IDComponent>().Id.str();
        ImGui::Image((ImTextureID)m_editor->entityIcon->GetInternalId(), ImVec2(48, 48));
        ImGui::SameLine();
        ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y +12 });
        ImGui::InputText(("##Label##" + entityIdString).c_str(), &ent.GetComponent<nimo::LabelComponent>().Label);
        ImGui::TextDisabled(entityIdString.c_str());
        ImGui::Spacing();
        if(ent.HasComponent<nimo::TransformComponent>())
        {
            if (ImGui::CollapsingHeader((std::string("Transform##")+entityIdString).c_str()))
            {
                std::string p = "Position##";
                p += entityIdString;
                std::string r = "Rotation##";
                r += entityIdString;
                std::string s = "Scale##";
                s += entityIdString;
                ImGui::DragFloat3(p.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Translation, 0.1f);
                ImGui::DragFloat3(r.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Rotation, 0.1f);
                ImGui::DragFloat3(s.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Scale, 0.1f);
            }
            ImGui::Spacing();
        }
        if(ent.HasComponent<nimo::CameraComponent>())
        {
            if (ImGui::CollapsingHeader((std::string("Camera##")+entityIdString).c_str()))
            {
                static int item_current_2 = 0;
                ImGui::Combo((std::string("Projection##") + (std::string("Camera##")+entityIdString)).c_str(), &item_current_2, "Perspective\0Orthographic\0\0");
                if(item_current_2 == 0)
                {
                    ent.GetComponent<nimo::CameraComponent>().Projection = nimo::CameraComponent::Projection::Perspective;
                    ImGui::SliderFloat((std::string("FOV##") + (std::string("Camera##")+entityIdString)).c_str(), &ent.GetComponent<nimo::CameraComponent>().FOV, 0.1f, 179.0f);
                }
                if(item_current_2 == 1)
                {
                    ent.GetComponent<nimo::CameraComponent>().Projection = nimo::CameraComponent::Projection::Orthographic;
                }
                ImGui::DragFloat((std::string("Clipping plane Far##") + (std::string("Camera##")+entityIdString)).c_str(), &ent.GetComponent<nimo::CameraComponent>().ClippingPlanes.Far, 0.1f, 0.0f);
                ImGui::DragFloat((std::string("Clipping plane Near##") + (std::string("Camera##")+entityIdString)).c_str(), &ent.GetComponent<nimo::CameraComponent>().ClippingPlanes.Near, 0.1f, 0.0f);
            }
            ImGui::Spacing();
        }
        if(ent.HasComponent<nimo::MeshRendererComponent>())
        {
            if (ImGui::CollapsingHeader((std::string("Mesh Renderer##")+entityIdString).c_str()))
            {
                if (ImGui::TreeNode((std::string("Material##") + (std::string("Mesh##")+entityIdString)).c_str()))
                {
                    auto mat = ent.GetComponent<nimo::MeshRendererComponent>().material;
                    for(auto p: mat->properties)
                    {
                        switch (p->type)
                        {
                        case nimo::MaterialPropertyType::Vector2:
                            {
                                ImGui::DragFloat2((p->name + "##" + (std::string("Material##")+ (std::string("Mesh Renderer##")+entityIdString))).c_str(), (float*)p->GetDataPtr(), 0.1f);
                            }
                            break;
                        case nimo::MaterialPropertyType::Texture:
                            {
                                ImGui::InputInt((p->name + "##" + (std::string("Material##")+ (std::string("Mesh Renderer##")+entityIdString))).c_str(), (int*)p->GetDataPtr());
                            }
                            break;
                        
                        default:
                            break;
                        }
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::Spacing();
        }
        if(ent.HasComponent<nimo::MeshComponent>())
        {
            if (ImGui::CollapsingHeader((std::string("Mesh##")+entityIdString).c_str()))
            {
                ImGui::Text((std::string("Mesh asset:").c_str()));
                ImGui::SameLine();
                ImGui::Text((std::string(nimo::AssetManager::GetMetadata(ent.GetComponent<nimo::MeshComponent>().source->id).filepath.string())).c_str());
            }
            ImGui::Spacing();
        }
    }
}

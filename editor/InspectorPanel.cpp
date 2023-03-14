#include "InspectorPanel.h"
#include "imgui.h"
#include "EditorLayer.h"
#include "core/Log.h"
#include "misc/cpp/imgui_stdlib.h"

void InspectorPanel::OnRender()
{
    if(!selectedItem.valid()) return;
    auto metadata = nimo::AssetManager::GetMetadata(selectedItem);
    if(metadata.id.valid())
    {
        ImGui::TextDisabled(nimo::AssetTypeToString(metadata.type));
        ImGui::TextDisabled(metadata.filepath.string().c_str());
        ImGui::TextDisabled(metadata.id.str().c_str());

        //Show asset widgets based on type
        switch (metadata.type)
        {
        case nimo::AssetType::Material:
            break;
        case nimo::AssetType::Mesh:
            break;
        case nimo::AssetType::Shader:
            break;
        case nimo::AssetType::Scene:
            break;
        case nimo::AssetType::Texture:
            break;
        default:
            break;
        }
        return;
    }
    for(auto scene : nimo::AssetManager::GetAllLoaded<nimo::Scene>())
    {
        auto ent = scene->GetEntity(selectedItem);
        auto entityIdString = ent.GetComponent<nimo::IDComponent>().Id.str();
        ImGui::Image((ImTextureID)m_editor->entityIcon->GetInternalId(), ImVec2(48, 48), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::SameLine();
        ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y +12 });
        ImGui::InputText(("##Label##" + entityIdString).c_str(), &ent.GetComponent<nimo::LabelComponent>().Label);
        ImGui::TextDisabled(entityIdString.c_str());
        ImGui::Spacing();
        if(ent.HasComponent<nimo::TransformComponent>())
        {
            if (ImGui::CollapsingHeader((std::string("Transform##")+entityIdString).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
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
            ImGui::Separator();
        }
        if(ent.HasComponent<nimo::CameraComponent>())
        {
            if (ImGui::CollapsingHeader((std::string("Camera##")+entityIdString).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
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
            ImGui::Separator();
        }
        if(ent.HasComponent<nimo::MeshRendererComponent>())
        {
            if (ImGui::CollapsingHeader((std::string("Mesh Renderer##")+entityIdString).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
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
            ImGui::Separator();
        }
        if(ent.HasComponent<nimo::MeshComponent>())
        {
            if (ImGui::CollapsingHeader((std::string("Mesh##")+entityIdString).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                // ImGui::Text((std::string("Mesh asset:").c_str()));
                // ImGui::SameLine();
                // ImGui::TextDisabled((std::string(nimo::AssetManager::GetMetadata(ent.GetComponent<nimo::MeshComponent>().source->id).filepath.string())).c_str());
                ImGui::Text("Mesh");
                ImGui::SameLine();
                ImGui::InputTextWithHint(("##Asset##Mesh##"+entityIdString).c_str(), "Drag mesh asset", &nimo::AssetManager::GetMetadata(ent.GetComponent<nimo::MeshComponent>().source->id).filepath.string(), ImGuiInputTextFlags_ReadOnly);
            }
            ImGui::Spacing();
            ImGui::Separator();
        }
        ImGui::Spacing();

        float alignment = 0.5f;
        const char* label = "\tAdd New Component...\t";
        ImGuiStyle& style = ImGui::GetStyle();

        float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;

        float off = (avail - size) * alignment;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

        if(ImGui::Button(label))
        {
            ImGui::OpenPopup("add_component_popup");
        }
        if (ImGui::BeginPopup("add_component_popup"))
        {
            ImGui::CollapsingHeader("\tComponents...", ImGuiTreeNodeFlags_Leaf);
            ImGui::Separator();
            if (ImGui::Selectable("Mesh Renderer")){
                ent.AddComponent<nimo::MeshComponent>().source = nimo::AssetManager::Get<nimo::MeshSource>("Objects/cube/cube.obj");
                ent.AddComponent<nimo::MeshRendererComponent>().material = nimo::AssetManager::Get<nimo::Material>("Materials/test.nmat");
                ent.GetComponent<nimo::MeshRendererComponent>().mesh = std::make_shared<nimo::Mesh>(ent.GetComponent<nimo::MeshComponent>().source);
            }
            if (ImGui::Selectable("Camera")){}
            ImGui::EndPopup();
        }
    }
}

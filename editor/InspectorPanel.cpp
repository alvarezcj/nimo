#include "InspectorPanel.h"
#include "imgui.h"
#include "EditorLayer.h"
#include "core/Log.h"
#include "misc/cpp/imgui_stdlib.h"
#include "scripting/ScriptManager.h"
#include "scene/Prefab.h"

void InspectorPanel::OnRender()
{
    if(!selectedItem.valid()) return;
    auto metadata = nimo::AssetManager::GetMetadata(selectedItem);
    if(metadata.id.valid())
    {
        ImGui::Image((ImTextureID)m_editor->assetIcons[metadata.type]->GetInternalId(), ImVec2(48, 48), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::SameLine();
        ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y +12 });
        ImGui::Text((metadata.filepath.string()).c_str());
        ImGui::TextDisabled(metadata.id.str().c_str());

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        //Show asset widgets based on type
        switch (metadata.type)
        {
        case nimo::AssetType::Material:
            // Show associated shader and properties
            {
                std::shared_ptr<nimo::Material> materialAsset = nimo::AssetManager::Get<nimo::Material>(metadata.id);

                auto shaders = nimo::AssetManager::GetAllExisting<nimo::Shader>();
                static nimo::AssetMetadata selectedShader;
                ImGui::Spacing();
                ImGui::Text("Shader");
                ImGui::SameLine();
                if(materialAsset->shader)
                    selectedShader = nimo::AssetManager::GetMetadata(materialAsset->shader->id);
                static bool shaderChanged = false;
                if (ImGui::BeginCombo("##Inspector##Asset##Material", selectedShader.filepath.string().c_str(), ImGuiComboFlags_None))
                {
                    for (int n = 0; n < shaders.size(); n++)
                    {
                        if (ImGui::Selectable(shaders[n].filepath.string().c_str()))
                        {
                            selectedShader = shaders[n];
                            shaderChanged = true;
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::SameLine();
                float alignment = 1.0f;
                const char* label = "\tApply\t";
                ImGuiStyle& style = ImGui::GetStyle();

                float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
                float avail = ImGui::GetContentRegionAvail().x;

                float off = (avail - size) * alignment;
                if (off > 0.0f)
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
                if(ImGui::Button(label))
                {
                    nimo::AssetManager::Serialize<nimo::Material>(materialAsset->id);
                }
                if(shaderChanged)
                {
                    shaderChanged = false;
                    nimo::AssetMetadata selectedShaderInfo = nimo::AssetManager::GetMetadata(selectedShader.id);
                    if(selectedShaderInfo.id.valid())
                    {
                        materialAsset->ClearProperties();
                        for(auto uniform : nimo::AssetManager::Get<nimo::Shader>(selectedShaderInfo.id)->GetUniforms())
                        {
                            if(uniform.name == "transform" || uniform.name == "view" || uniform.name == "projection") continue;
                            NIMO_DEBUG("Adding {} material {} property", metadata.filepath.string(), uniform.name);
                            materialAsset->AddProperty(uniform.name, uniform.type);
                        }
                        NIMO_DEBUG("Serializing {}", metadata.filepath.string());
                        materialAsset->shader = nimo::AssetManager::Get<nimo::Shader>(selectedShaderInfo.id);
                        //nimo::AssetManager::Serialize<nimo::Material>(materialAsset->id);
                    }
                }
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::Text("Properties");
                ImGui::Spacing();
                for(auto p: materialAsset->properties)
                {
                    switch (p->type)
                    {
                    case nimo::ShaderUniformDataType::Float2:
                        {
                            ImGui::DragFloat2((p->name + "##Inspector##Asset##Material1##" +materialAsset->id.str()).c_str(), (float*)p->GetDataPtr(), 0.01f);
                        }
                        break;
                    case nimo::ShaderUniformDataType::Float4:
                        {
                            ImGui::DragFloat4((p->name + "##Inspector##Asset##Material2##" +materialAsset->id.str()).c_str(), (float*)p->GetDataPtr(), 0.01f);
                        }
                        break;
                    case nimo::ShaderUniformDataType::Sampler2D:
                        {
                            ImGui::InputInt((p->name + "##Inspector##Asset##Material3##TextureSlot##" +materialAsset->id.str()).c_str(), (int*)p->GetDataPtr());

                            static std::string filepath;
                            if (((nimo::MaterialPropertyTexture*)p)->GetTexture())
                                ImGui::InputTextWithHint((p->name + "##Inspector##Asset##Material4##TextureAsset##" + materialAsset->id.str()).c_str(), "Drag Texture asset", &nimo::AssetManager::GetMetadata(((nimo::MaterialPropertyTexture*)p)->GetTexture()->id).filepath.string(), ImGuiInputTextFlags_ReadOnly);
                            else
                                ImGui::InputTextWithHint((p->name + "##Inspector##Asset##Material4##TextureAsset##" + materialAsset->id.str()).c_str(), "Drag Texture asset", &filepath, ImGuiInputTextFlags_ReadOnly);

                            if (ImGui::BeginDragDropTarget())
                            {
                                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NIMO_ASSET_FILE"))
                                {
                                    std::filesystem::path payloadPath = std::string((char*)payload->Data);
                                    auto info = nimo::AssetManager::GetMetadata(payloadPath);
                                    if(info.id.valid() && info.type == nimo::AssetType::Texture) // Found in asset manager
                                    {
                                        ((nimo::MaterialPropertyTexture*)p)->SetTexture(nimo::AssetManager::Get<nimo::Texture>(info.id));
                                    }
                                }
                                ImGui::EndDragDropTarget();
                            }
                        }
                        break;
                    
                    default:
                        break;
                    }
                }
            }
            break;
        case nimo::AssetType::Mesh:
            // Show vertices, submeshes, indices,...
            {
                std::shared_ptr<nimo::Mesh> meshAsset = nimo::AssetManager::Get<nimo::Mesh>(metadata.id);
                ImGui::Text("Vertices: %d", meshAsset->m_vertices.size());
                ImGui::Text("Indices: %d", meshAsset->m_indices.size());
                ImGui::Separator();
            }
            break;
        case nimo::AssetType::Shader:
            // Show source code for fragment and vertex. Allow for modification
            {
                std::shared_ptr<nimo::Shader> shaderAsset = nimo::AssetManager::Get<nimo::Shader>(metadata.id);
                ImGui::Text("Vertex Shader Code");
                ImGui::SameLine();
                float alignment = 1.0f;
                const char* label = "\tApply\t";
                ImGuiStyle& style = ImGui::GetStyle();

                float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
                float avail = ImGui::GetContentRegionAvail().x;

                float off = (avail - size) * alignment;
                if (off > 0.0f)
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
                if(ImGui::Button(label))
                {
                    nimo::AssetManager::Serialize<nimo::Shader>(shaderAsset->id);
                    shaderAsset->Recompile();
                }
                ImGui::Spacing();
                ImGui::InputTextMultiline("##Shader##VertexCode", shaderAsset->GetVertexCodePtr(), ImVec2(ImGui::GetContentRegionAvailWidth(), 500.f), ImGuiInputTextFlags_AllowTabInput);
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::Text("Fragment Shader Code");
                ImGui::Spacing();
                ImGui::InputTextMultiline("##Shader##FragmentCode", shaderAsset->GetFragmentCodePtr(), ImVec2(ImGui::GetContentRegionAvailWidth(), 500.f), ImGuiInputTextFlags_AllowTabInput);
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
            }
            break;
        case nimo::AssetType::Scene:
            // Show nothing
            break;
        case nimo::AssetType::Texture:
            // Show image, width, height,...
            {
                std::shared_ptr<nimo::Texture> textureAsset = nimo::AssetManager::Get<nimo::Texture>(metadata.id);
                ImGui::Text("Size: %dx%d", textureAsset->Width(), textureAsset->Height());
                ImGui::Spacing();
                ImGui::Text("Preview");
                float offset = 20.0f;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset/2.0f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset/2.0f);
                ImGui::Image((ImTextureID)textureAsset->GetInternalId(), ImVec2(ImGui::GetContentRegionAvailWidth() - offset, (ImGui::GetContentRegionAvailWidth()- offset)*(float)textureAsset->Height()/(float)textureAsset->Width()), ImVec2(0, 1), ImVec2(1, 0));
            }
            break;
        case nimo::AssetType::EnvironmentMap:
            // Show nothing
            {
                std::shared_ptr<nimo::EnvironmentMap> textureAsset = nimo::AssetManager::Get<nimo::EnvironmentMap>(metadata.id);
                ImGui::Text("Size: %dx%d", textureAsset->Width(), textureAsset->Height());
                ImGui::Spacing();
                ImGui::Text("Preview");
                float offset = 20.0f;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset/2.0f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset/2.0f);
                ImGui::Image((ImTextureID)textureAsset->GetInternalId(), ImVec2(ImGui::GetContentRegionAvailWidth() - offset, (ImGui::GetContentRegionAvailWidth()- offset)*(float)textureAsset->Height()/(float)textureAsset->Width()), ImVec2(0, 1), ImVec2(1, 0));
            }
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
        static bool activeValue = ent.GetComponent<nimo::ActiveComponent>().active;
        if(ImGui::Checkbox(("##Active##" + entityIdString).c_str(), &activeValue))
        {
            scene->SetEntityActive(ent, activeValue);
        }
        ImGui::SameLine();
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
                ImGui::DragFloat3(p.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Translation, 0.01f);
                ImGui::DragFloat3(r.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Rotation, 0.01f);
                ImGui::DragFloat3(s.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Scale, 0.01f);
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
                ImGui::Text("Material");
                ImGui::SameLine();
                auto material = ent.GetComponent<nimo::MeshRendererComponent>().material;
                std::string filepath;
                if (material)
                    ImGui::InputTextWithHint(("##Asset##Material##" + entityIdString).c_str(), "Drag Material asset", &nimo::AssetManager::GetMetadata(ent.GetComponent<nimo::MeshRendererComponent>().material->id).filepath.string(), ImGuiInputTextFlags_ReadOnly);
                else
                    ImGui::InputTextWithHint(("##Asset##Material##" + entityIdString).c_str(), "Drag Material asset", &filepath, ImGuiInputTextFlags_ReadOnly);

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NIMO_ASSET_FILE"))
                    {
                        std::filesystem::path payloadPath = std::string((char*)payload->Data);
                        auto info = nimo::AssetManager::GetMetadata(payloadPath);
                        if(info.id.valid() && info.type == nimo::AssetType::Material) // Found in asset manager
                        {
                            ent.GetComponent<nimo::MeshRendererComponent>().material = nimo::AssetManager::Get<nimo::Material>(info.id);
                        }
                    }
                    ImGui::EndDragDropTarget();
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
                auto mesh = ent.GetComponent<nimo::MeshComponent>().source;
                std::string filepath;
                if (mesh)
                    ImGui::InputTextWithHint(("##Asset##Mesh##"+entityIdString).c_str(), "Drag mesh asset", &nimo::AssetManager::GetMetadata(ent.GetComponent<nimo::MeshComponent>().source->id).filepath.string(), ImGuiInputTextFlags_ReadOnly);
                else
                    ImGui::InputTextWithHint(("##Asset##Mesh##" + entityIdString).c_str(), "Drag mesh asset", &filepath, ImGuiInputTextFlags_ReadOnly);
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NIMO_ASSET_FILE"))
                    {
                        std::filesystem::path payloadPath = std::string((char*)payload->Data);
                        auto info = nimo::AssetManager::GetMetadata(payloadPath);
                        if(info.id.valid() && info.type == nimo::AssetType::Mesh) // Found in asset manager
                        {
                            ent.GetComponent<nimo::MeshComponent>().source = nimo::AssetManager::Get<nimo::Mesh>(info.id);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
            }
            ImGui::Spacing();
            ImGui::Separator();
        }
        if(ent.HasComponent<nimo::PointLightComponent>())
        {
            if (ImGui::CollapsingHeader((std::string("Point Light##")+entityIdString).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::ColorEdit3(("Color##Point Light##" + entityIdString).c_str(), (float*)&ent.GetComponent<nimo::PointLightComponent>().Color, ImGuiColorEditFlags_Float);
                ImGui::DragFloat(("Intesity##Point Light##" + entityIdString).c_str(), (float*)&ent.GetComponent<nimo::PointLightComponent>().Intensity, 0.1f, 0.0f, 100.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
            }
            ImGui::Spacing();
            ImGui::Separator();
        }
        if(ent.HasComponent<nimo::ScriptComponent>())
        {
            for(auto instance : ent.GetComponent<nimo::ScriptComponent>().instances)
            {
                if (ImGui::CollapsingHeader((std::filesystem::path(instance.script->filepath).stem().string()+"##"+std::to_string(instance.stackReference)+entityIdString).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    for(auto& field : instance.fields)
                    {
                        ImGui::Text(field.first.c_str());
                        ImGui::SameLine();
                        ImGui::Spacing();
                        ImGui::SameLine();
                        switch (field.second->GetType())
                        {
                        case nimo::ScriptFieldType::Number:
                            ImGui::DragFloat(
                                ("##" + field.first + "##" + (std::filesystem::path(instance.script->filepath).stem().string()+"##"+std::to_string(instance.stackReference)+entityIdString)).c_str(),
                                (float*)&std::static_pointer_cast<nimo::ScriptFieldNumber>(field.second)->value);
                            break;
                        case nimo::ScriptFieldType::Boolean:
                            ImGui::Checkbox(
                                ("##" + field.first + "##" + (std::filesystem::path(instance.script->filepath).stem().string()+"##"+std::to_string(instance.stackReference)+entityIdString)).c_str(),
                                &std::static_pointer_cast<nimo::ScriptFieldBool>(field.second)->value);
                            break;
                        case nimo::ScriptFieldType::String:
                            ImGui::InputText(
                                ("##" + field.first + "##" + (std::filesystem::path(instance.script->filepath).stem().string()+"##"+std::to_string(instance.stackReference)+entityIdString)).c_str(),
                                &std::static_pointer_cast<nimo::ScriptFieldString>(field.second)->value);
                            break;
                        case nimo::ScriptFieldType::Asset:
                        {
                            auto asset = std::static_pointer_cast<nimo::ScriptFieldAsset>(field.second)->value;
                            switch (std::static_pointer_cast<nimo::ScriptFieldAsset>(field.second)->type)
                            {
                            case nimo::AssetType::Prefab:
                                {
                                    std::string filepath;
                                    if (asset)
                                        ImGui::InputTextWithHint(("##Asset##" + field.first + "##" + (std::filesystem::path(instance.script->filepath).stem().string()+"##"+std::to_string(instance.stackReference)+entityIdString)).c_str(), "Drag prefab asset", &nimo::AssetManager::GetMetadata(asset->id).filepath.string(), ImGuiInputTextFlags_ReadOnly);
                                    else
                                        ImGui::InputTextWithHint(("##Asset##" + field.first + "##" + (std::filesystem::path(instance.script->filepath).stem().string()+"##"+std::to_string(instance.stackReference)+entityIdString)).c_str(), "Drag prefab asset", &filepath, ImGuiInputTextFlags_ReadOnly);
                                    if (ImGui::BeginDragDropTarget())
                                    {
                                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NIMO_ASSET_FILE"))
                                        {
                                            std::filesystem::path payloadPath = std::string((char*)payload->Data);
                                            auto info = nimo::AssetManager::GetMetadata(payloadPath);
                                            if(info.id.valid() && info.type == nimo::AssetType::Prefab) // Found in asset manager
                                            {
                                                std::static_pointer_cast<nimo::ScriptFieldAsset>(field.second)->value = std::static_pointer_cast<nimo::Asset>(nimo::AssetManager::Get<nimo::Prefab>(info.id));
                                            }
                                        }
                                        ImGui::EndDragDropTarget();
                                    }
                                }
                                break;
                            case nimo::AssetType::Material:
                                {
                                    std::string filepath;
                                    if (asset)
                                        ImGui::InputTextWithHint(("##Asset##" + field.first + "##" + (std::filesystem::path(instance.script->filepath).stem().string()+"##"+std::to_string(instance.stackReference)+entityIdString)).c_str(), "Drag prefab asset", &nimo::AssetManager::GetMetadata(asset->id).filepath.string(), ImGuiInputTextFlags_ReadOnly);
                                    else
                                        ImGui::InputTextWithHint(("##Asset##" + field.first + "##" + (std::filesystem::path(instance.script->filepath).stem().string()+"##"+std::to_string(instance.stackReference)+entityIdString)).c_str(), "Drag prefab asset", &filepath, ImGuiInputTextFlags_ReadOnly);
                                    if (ImGui::BeginDragDropTarget())
                                    {
                                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NIMO_ASSET_FILE"))
                                        {
                                            std::filesystem::path payloadPath = std::string((char*)payload->Data);
                                            auto info = nimo::AssetManager::GetMetadata(payloadPath);
                                            if(info.id.valid() && info.type == nimo::AssetType::Material) // Found in asset manager
                                            {
                                                std::static_pointer_cast<nimo::ScriptFieldAsset>(field.second)->value = std::static_pointer_cast<nimo::Asset>(nimo::AssetManager::Get<nimo::Material>(info.id));
                                            }
                                        }
                                        ImGui::EndDragDropTarget();
                                    }
                                }
                                break;
                            default:
                                break;
                            }
                            break;
                        }
                        default:
                            break;
                        }
                    }
                }
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
            if (ImGui::MenuItem("Mesh")){
                ent.AddComponent<nimo::MeshComponent>();
            }
            if (ImGui::MenuItem("Mesh Renderer")){
                ent.AddComponent<nimo::MeshRendererComponent>();
            }
            if (ImGui::MenuItem("Point Light")){
                ent.AddComponent<nimo::PointLightComponent>();
            }
            if (ImGui::MenuItem("Camera")){}
            if(ImGui::BeginMenu("Scripts"))
            {
                auto scripts = nimo::AssetManager::GetAllExisting<nimo::Script>();
                for(int i = 0; i < scripts.size(); ++i)
                {
                    if (ImGui::MenuItem(scripts[i].filepath.stem().string().c_str())){
                        if(!ent.HasComponent<nimo::ScriptComponent>())
                            ent.AddComponent<nimo::ScriptComponent>();
                        ent.GetComponent<nimo::ScriptComponent>().instances.push_back(nimo::ScriptManager::CreateInstance(nimo::AssetManager::Get<nimo::Script>(scripts[i].id), ent.ID(), scene));
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }
    }
}

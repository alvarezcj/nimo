#include "SceneContentsPanel.h"
#include "imgui.h"
#include "EditorLayer.h"
#include "core/Log.h"

void SceneContentsPanel::PaintEntity(const std::shared_ptr<nimo::Scene>& scene, nimo::Entity& ent)
{
    const static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen;
    ImGuiTreeNodeFlags node_flags = base_flags;
    if(selectedEntity == ent.GetComponent<nimo::IDComponent>().Id)
        node_flags |= ImGuiTreeNodeFlags_Selected;

    std::string entityName =  ent.GetComponent<nimo::LabelComponent>().Label;
    std::string entityIdString =  ent.GetComponent<nimo::IDComponent>().Id.str();
    bool open;

    // Paint as tree or leaf
    if(ent.GetComponent<nimo::FamilyComponent>().Children.size() != 0) // Has children
    {
        open = ImGui::TreeNodeEx((entityName + "##" + scene->id.str()).c_str(), node_flags);
    }
    else // No children, paint as leaf node
    {
        node_flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Leaf; // ImGuiTreeNodeFlags_Bullet
        ImGui::TreeNodeEx((entityName + "##" + scene->id.str()).c_str(), node_flags);
    }

    // Check if selected
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        selectedEntity = ent.GetComponent<nimo::IDComponent>().Id;
        NIMO_DEBUG("Selected entity: {}", entityName);
    }

    // Drag n drop
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
    {
        ImGui::SetDragDropPayload("NIMO_ENTITY_ID", &ent.GetComponent<nimo::IDComponent>().Id, sizeof(nimo::GUID));
        ImGui::Text(entityName.c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NIMO_ENTITY_ID"))
        {
            IM_ASSERT(payload->DataSize == sizeof(nimo::GUID));
            nimo::GUID payload_n = *(const nimo::GUID*)payload->Data;
            NIMO_DEBUG("Received drag drop entity: {}", payload_n.str());
            auto payloadEntity = scene->GetEntity(payload_n);
            if(payloadEntity.GetComponent<nimo::FamilyComponent>().Parent.valid())
            {
                auto payloadParent = scene->GetEntity(payloadEntity.GetComponent<nimo::FamilyComponent>().Parent);
                NIMO_DEBUG("Removing {} as child from {}", payloadEntity.GetComponent<nimo::LabelComponent>().Label, payloadParent.GetComponent<nimo::LabelComponent>().Label);
                for(int i = 0; i< payloadParent.GetComponent<nimo::FamilyComponent>().Children.size(); ++i)
                {
                    if(payloadParent.GetComponent<nimo::FamilyComponent>().Children[i] == payload_n)
                    {
                        payloadParent.GetComponent<nimo::FamilyComponent>().Children.erase(payloadParent.GetComponent<nimo::FamilyComponent>().Children.begin() + i);
                        break;
                    }
                }
                NIMO_DEBUG("Entiy {} has now {} children", payloadParent.GetComponent<nimo::LabelComponent>().Label, payloadParent.GetComponent<nimo::FamilyComponent>().Children.size());
            }
            payloadEntity.GetComponent<nimo::FamilyComponent>().Parent = ent.GetComponent<nimo::IDComponent>().Id;
            ent.GetComponent<nimo::FamilyComponent>().Children.push_back(payload_n);
        }
        ImGui::EndDragDropTarget();
    }

    // Paint children recursively
    if(open)
    {
        // Paint children
        for(auto child : ent.GetComponent<nimo::FamilyComponent>().Children)
        {
            PaintEntity(scene, scene->GetEntity(child));
        }
        if(ent.GetComponent<nimo::FamilyComponent>().Children.size())
            ImGui::TreePop();
    }
}

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
                if(!ent.GetComponent<nimo::FamilyComponent>().Parent.valid())
                    this->PaintEntity(scene.second, ent);
                // if (ImGui::TreeNode((entityName + "##" + scene.second->id.str()).c_str()))
                // {
                //     totalHeight += ImGui::GetItemRectSize().y;
                //     if(ent.HasComponent<nimo::TransformComponent>())
                //     {
                //         if (ImGui::TreeNode((std::string("Transform##")+entityName).c_str()))
                //         {
                //             totalHeight += ImGui::GetItemRectSize().y;
                //             std::string p = "Position##";
                //             p += entityIdString;
                //             std::string r = "Rotation##";
                //             r += entityIdString;
                //             std::string s = "Scale##";
                //             s += entityIdString;
                //             ImGui::DragFloat3(p.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Translation, 0.1f);
                //             totalHeight += ImGui::GetItemRectSize().y;
                //             ImGui::DragFloat3(r.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Rotation, 0.1f);
                //             totalHeight += ImGui::GetItemRectSize().y;
                //             ImGui::DragFloat3(s.c_str(), (float*)&ent.GetComponent<nimo::TransformComponent>().Scale, 0.1f);
                //             totalHeight += ImGui::GetItemRectSize().y;
                //             ImGui::TreePop();
                //         }
                //         else{
                //             totalHeight += ImGui::GetItemRectSize().y;
                //         }
                //     }
                //     if(ent.HasComponent<nimo::CameraComponent>())
                //     {
                //         if (ImGui::TreeNode((std::string("Camera##")+entityName).c_str()))
                //         {
                //             totalHeight += ImGui::GetItemRectSize().y;
                //             static int item_current_2 = 0;
                //             ImGui::Combo((std::string("Projection##") + (std::string("Camera##")+entityName)).c_str(), &item_current_2, "Perspective\0Orthographic\0\0");
                //             totalHeight += ImGui::GetItemRectSize().y;
                //             if(item_current_2 == 0)
                //             {
                //                 ent.GetComponent<nimo::CameraComponent>().Projection = nimo::CameraComponent::Projection::Perspective;
                //                 ImGui::SliderFloat((std::string("FOV##") + (std::string("Camera##")+entityName)).c_str(), &ent.GetComponent<nimo::CameraComponent>().FOV, 0.1f, 179.0f);
                //                 totalHeight += ImGui::GetItemRectSize().y;
                //             }
                //             if(item_current_2 == 1)
                //             {
                //                 ent.GetComponent<nimo::CameraComponent>().Projection = nimo::CameraComponent::Projection::Orthographic;
                //             }
                //             ImGui::DragFloat((std::string("Clipping plane Far##") + (std::string("Camera##")+entityName)).c_str(), &ent.GetComponent<nimo::CameraComponent>().ClippingPlanes.Far, 0.1f, 0.0f);
                //             totalHeight += ImGui::GetItemRectSize().y;
                //             ImGui::DragFloat((std::string("Clipping plane Near##") + (std::string("Camera##")+entityName)).c_str(), &ent.GetComponent<nimo::CameraComponent>().ClippingPlanes.Near, 0.1f, 0.0f);
                //             totalHeight += ImGui::GetItemRectSize().y;

                //             ImGui::TreePop();
                //         }
                //         else{
                //             totalHeight += ImGui::GetItemRectSize().y;
                //         }
                //     }
                //     if(ent.HasComponent<nimo::MeshRendererComponent>())
                //     {
                //         if (ImGui::TreeNode((std::string("Mesh Renderer##")+entityName).c_str()))
                //         {
                //             totalHeight += ImGui::GetItemRectSize().y;
                //             if (ImGui::TreeNode((std::string("Material##") + (std::string("Mesh##")+entityName)).c_str()))
                //             {
                //                 totalHeight += ImGui::GetItemRectSize().y;
                //                 auto mat = ent.GetComponent<nimo::MeshRendererComponent>().material;
                //                 for(auto p: mat->properties)
                //                 {
                //                     switch (p->type)
                //                     {
                //                     case nimo::MaterialPropertyType::Vector2:
                //                         {
                //                             ImGui::DragFloat2((p->name + "##" + (std::string("Material##")+ (std::string("Mesh Renderer##")+entityName))).c_str(), (float*)p->GetDataPtr(), 0.1f);
                //                             totalHeight += ImGui::GetItemRectSize().y;
                //                         }
                //                         break;
                //                     case nimo::MaterialPropertyType::Texture:
                //                         {
                //                             ImGui::InputInt((p->name + "##" + (std::string("Material##")+ (std::string("Mesh Renderer##")+entityName))).c_str(), (int*)p->GetDataPtr());
                //                             totalHeight += ImGui::GetItemRectSize().y;
                //                         }
                //                         break;
                                    
                //                     default:
                //                         break;
                //                     }
                //                 }
                //                 ImGui::TreePop();
                //             }

                //         else{
                //             totalHeight += ImGui::GetItemRectSize().y;
                //         }
                //             ImGui::TreePop();
                //         }
                //         else{
                //             totalHeight += ImGui::GetItemRectSize().y;
                //         }
                //     }
                //     if(ent.HasComponent<nimo::MeshComponent>())
                //     {
                //         if (ImGui::TreeNode((std::string("Mesh##")+entityName).c_str()))
                //         {
                //             totalHeight += ImGui::GetItemRectSize().y;
                //             ImGui::Text((std::string("Mesh asset:").c_str()));
                //             ImGui::SameLine();
                //             ImGui::Text((std::string(nimo::AssetManager::GetMetadata(ent.GetComponent<nimo::MeshComponent>().source->id).filepath.string())).c_str());
                //             totalHeight += ImGui::GetItemRectSize().y;

                //             ImGui::TreePop();
                //         }
                //         else{
                //             totalHeight += ImGui::GetItemRectSize().y;
                //         }
                //     }
                //     ImGui::TreePop();
                // }
                // else{
                //     totalHeight += ImGui::GetItemRectSize().y;
                // }

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
            nimo::Entity parent = scene.second->CreateEntity();
            nimo::Entity childCube = scene.second->CreateEntity();
            childCube.GetComponent<nimo::FamilyComponent>().Parent = parent.GetComponent<nimo::IDComponent>().Id;
            childCube.AddComponent<nimo::TransformComponent>().Translation = {1.0f, 1.0f, -3.0f};
            childCube.AddComponent<nimo::MeshComponent>().source = nimo::AssetManager::Get<nimo::MeshSource>("Objects/cube/cube.obj");
            childCube.AddComponent<nimo::MeshRendererComponent>().material = nimo::AssetManager::Get<nimo::Material>("Materials/test.nmat");
            childCube.GetComponent<nimo::MeshRendererComponent>().mesh = std::make_shared<nimo::Mesh>(childCube.GetComponent<nimo::MeshComponent>().source);

            parent.GetComponent<nimo::FamilyComponent>().Children.push_back(childCube.GetComponent<nimo::IDComponent>().Id);
        }
    }
}

#include "SceneContentsPanel.h"
#include "imgui.h"
#include "EditorLayer.h"
#include "core/Log.h"
#include "InspectorPanel.h"
#include "scene/Prefab.h"

void SceneContentsPanel::PaintEntity(const std::shared_ptr<nimo::Scene>& scene, nimo::Entity& ent)
{
    const static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen;
    ImGuiTreeNodeFlags node_flags = base_flags;
    if(selectedEntity == ent.GetComponent<nimo::IDComponent>().Id)
        node_flags |= ImGuiTreeNodeFlags_Selected;

    std::string entityName =  ent.GetComponent<nimo::LabelComponent>().Label;
    std::string entityIdString =  ent.GetComponent<nimo::IDComponent>().Id.str();
    bool open = false;

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

    // Popup right click
    bool mustDestroy = false;
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Selectable("Create Empty")){
            scene->CreateEntityWithParent(ent);
        }
        if (ImGui::Selectable("Delete")){
            mustDestroy = true;
        }
        ImGui::Separator();
        if (ImGui::Selectable("Create Prefab")){
            auto prefab = std::make_shared<nimo::Prefab>();
            prefab->SetData(scene, ent);
            nimo::AssetManager::CreateAssetFromMemory<nimo::Prefab>(entityName + ".nprefab", prefab);
            nimo::AssetManager::WriteIndex();
        }
        ImGui::EndPopup();
    }
    if(mustDestroy)
    {
        if(open) ImGui::TreePop();
        scene->DestroyEntity(ent);
        m_editor->inspectorPanel->ResetViewItem();
        return;
    }

    // Check if selected
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered() && !ImGui::IsItemToggledOpen())
    {
        selectedEntity = ent.GetComponent<nimo::IDComponent>().Id;
        m_editor->inspectorPanel->SetViewItem(selectedEntity);
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
    ImRect windowRect = { ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax() };
    for(auto scene : nimo::AssetManager::GetAllLoaded<nimo::Scene>())
    {
        float totalHeight = 0;
        ImVec2 rectMin;
        ImVec2 rectMax;
        bool res;
        bool openHeader = ImGui::CollapsingHeader((scene->GetName() + "##" + scene->id.str()).c_str(), ImGuiTreeNodeFlags_DefaultOpen);
        if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
        {
            if (ImGui::Selectable("Create Empty")){
                scene->CreateEntity();
            }
            ImGui::EndPopup();
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
                payloadEntity.GetComponent<nimo::FamilyComponent>().Parent = nimo::GUID();
            }
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NIMO_ASSET_FILE"))
            {
                std::filesystem::path payloadPath = std::string((char*)payload->Data);
                auto info = nimo::AssetManager::GetMetadata(payloadPath);
                if(info.id.valid() && info.type == nimo::AssetType::Prefab) // Found in asset manager
                {
                    nimo::AssetManager::Get<nimo::Prefab>(info.id)->Create(scene);
                }
            }
            ImGui::EndDragDropTarget();
        }
        if(openHeader)
        {
            rectMin = ImGui::GetItemRectMin();
            rectMax = ImGui::GetItemRectMax();
            totalHeight += ImGui::GetItemRectSize().y;
            scene->ForEachEntity([&](nimo::Entity& ent)
            {
                if(!ent.GetComponent<nimo::FamilyComponent>().Parent.valid())
                    this->PaintEntity(scene, ent);
            });
        }
        else{
            totalHeight += ImGui::GetItemRectSize().y;
        }
        res = ImGui::IsMouseHoveringRect(rectMin, ImVec2(rectMax.x, rectMax.y + totalHeight)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
        ImGui::Separator();
        if(res)
        {
            m_editor->lastModifiedScene = scene->id;
        }
    }

}

#include "AssetExplorerPanel.h"
#include "imgui.h"
#include "project/Project.h"
#include "core/Log.h"
#include "core/FileHandling.h"
#include <functional>
#include "InspectorPanel.h"
#include "EditorLayer.h"

const static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_FramePadding;
void AssetExplorerPanel::OnRender()
{
    if(nimo::Project::GetActiveProject())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 2.0f));
        auto path = nimo::Project::GetActiveProject()->GetAssetsFolderPath();
            PaintDirectory(path);
        ImGui::PopStyleVar();
    }
}

void AssetExplorerPanel::PaintDirectory(const std::filesystem::path& path)
{
    ImGuiTreeNodeFlags node_flags = base_flags;
    if(selectedPath == path)
        node_flags |= ImGuiTreeNodeFlags_Selected;
    bool open = ImGui::TreeNodeEx(("##" + path.filename().string()).c_str(), node_flags);
    // Check if selected
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered() && !ImGui::IsItemToggledOpen())
    {
        selectedPath = path;
        NIMO_DEBUG("Selected asset: {}", path.string());
    }
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
    {
        ImGui::SetDragDropPayload("NIMO_ASSET_FOLDER", path.string().c_str(), path.string().size() + 1);
        ImGui::Text(path.filename().string().c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NIMO_ASSET_FOLDER"))
        {
            std::filesystem::path payloadPath = std::string((char*)payload->Data);
            NIMO_DEBUG("Received drag drop folder: {}", payloadPath.string());
            std::vector<std::pair<std::filesystem::path ,nimo::AssetId>> toUpdate;
            for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(payloadPath))
            {
                auto info = nimo::AssetManager::GetMetadata(dir_entry.path());
                if(info.id.valid()) // Found in asset manager
                {
                    toUpdate.push_back(std::make_pair(dir_entry.path().lexically_relative(payloadPath), info.id));
                }
            }
            if(nimo::FileHandling::Move(payloadPath, path / payloadPath.filename()))
            {
                for (auto const& toUpdatePair : toUpdate)
                {
                    nimo::AssetManager::UpdatePath(toUpdatePair.second, path / payloadPath.filename()/toUpdatePair.first);
                    auto info = nimo::AssetManager::GetMetadata(toUpdatePair.second);
                    NIMO_INFO("Updated filepath in registry {}", info.filepath.string());
                }
            }
        }
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("NIMO_ASSET_FILE"))
        {
            std::filesystem::path payloadPath = std::string((char*)payload->Data);
            if(nimo::FileHandling::Move(payloadPath, path / payloadPath.filename()))
            {
                auto info = nimo::AssetManager::GetMetadata(payloadPath);
                if(info.id.valid()) // Found in asset manager
                {
                    nimo::AssetManager::UpdatePath(info.id, path / payloadPath.filename());
                    info = nimo::AssetManager::GetMetadata(info.id);
                    NIMO_INFO("Updated filepath in registry {}", info.filepath.string());
                }
            }
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::SameLine();
    ImGui::Image((ImTextureID)folderIcon->GetInternalId(), ImVec2(28,28), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SameLine();
    ImGui::Text(path.filename().string().c_str());

    if (open)
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                PaintDirectory(entry.path());
            }
            else {
                ImGuiTreeNodeFlags leaf_flags = base_flags;
                leaf_flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;
                if(selectedPath == entry.path())
                    leaf_flags |= ImGuiTreeNodeFlags_Selected;
                ImGui::TreeNodeEx(("##" + entry.path().filename().string()).c_str(), leaf_flags);
                
                // Drag/drop source
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    ImGui::SetDragDropPayload("NIMO_ASSET_FILE", entry.path().string().c_str(), entry.path().string().size() + 1);
                    auto info = nimo::AssetManager::GetMetadata(entry.path());
                    if(info.id.valid()) // Found in asset manager
                    {
                        ImGui::SetDragDropPayload((std::string("NIMO_ASSET_")+nimo::AssetTypeToString(info.type)).c_str(), &info.id, sizeof(nimo::AssetId));
                    }
                    ImGui::Text(entry.path().filename().string().c_str());
                    ImGui::EndDragDropSource();
                }
                // Check if selected
                if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered() && !ImGui::IsItemToggledOpen())
                {
                    selectedPath = entry.path();
                    auto info = nimo::AssetManager::GetMetadata(entry.path());
                    if(info.id.valid()) // Found in asset manager
                    {
                        m_editor->inspectorPanel->SetViewItem(info.id);
                    }
                    NIMO_DEBUG("Selected asset: {}", entry.path().string());
                }
                ImGui::SameLine();
                if(entry.path().has_extension())
                {
                    auto it = AssetExplorerPanel::mapExtensionIcon.find(entry.path().extension().string());
                    if(it != AssetExplorerPanel::mapExtensionIcon.end())
                    {
                        ImGui::Image((ImTextureID)it->second->GetInternalId(), ImVec2(28,28), ImVec2(0, 1), ImVec2(1, 0));
                    }
                    else{
                        ImGui::Image((ImTextureID)fileIcon->GetInternalId(), ImVec2(28,28), ImVec2(0, 1), ImVec2(1, 0));
                    }
                }
                else
                    ImGui::Image((ImTextureID)fileIcon->GetInternalId(), ImVec2(28,28), ImVec2(0, 1), ImVec2(1, 0));
                ImGui::SameLine();
                ImGui::Text(entry.path().filename().string().c_str());
            }
        }
        ImGui::TreePop();
    }
}
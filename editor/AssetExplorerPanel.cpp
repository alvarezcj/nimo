#include "AssetExplorerPanel.h"
#include "imgui.h"
#include "project/Project.h"
#include "core/Log.h"
#include <functional>

const static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
void AssetExplorerPanel::OnRender()
{
    if(nimo::Project::GetActiveProject())
    {
        auto path = nimo::Project::GetActiveProject()->GetAssetsFolderPath();
            PaintDirectory(path);
    }
}

void AssetExplorerPanel::PaintDirectory(const std::filesystem::path& path)
{
    ImGuiTreeNodeFlags node_flags = base_flags;
    bool open = ImGui::TreeNodeEx(("##" + path.filename().string()).c_str(), node_flags);
    ImGui::SameLine();
    ImGui::Image((ImTextureID)folderIcon->GetInternalId(), ImVec2(24, 24), ImVec2(0, 1), ImVec2(1, 0));
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
                node_flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;
                ImGui::TreeNodeEx(("##" + entry.path().filename().string()).c_str(), node_flags);
                ImGui::SameLine();
                ImGui::Image((ImTextureID)fileIcon->GetInternalId(), ImVec2(24, 24), ImVec2(0, 1), ImVec2(1, 0));
                ImGui::SameLine();
                ImGui::Text(entry.path().filename().string().c_str());
            }
        }
        ImGui::TreePop();
    }
}
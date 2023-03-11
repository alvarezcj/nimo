#pragma once
#include "EditorPanel.h"
#include "scene/Scene.h"
#include "renderer/Texture.h"

#include <memory>

class EditorLayer;
struct AssetExplorerPanel : public EditorPanel
{
    AssetExplorerPanel(EditorLayer* editor) 
        : m_editor(editor)
        , EditorPanel("AssetExplorer")
        , folderIcon(std::make_shared<nimo::Texture>("icon_folder.png"))
        , fileIcon(std::make_shared<nimo::Texture>("icon_file.png"))
    {}
    void PaintDirectory(const std::filesystem::path& path);
private:
    void OnRender();
    EditorLayer* m_editor;
    std::shared_ptr<nimo::Texture> folderIcon;
    std::shared_ptr<nimo::Texture> fileIcon;
};
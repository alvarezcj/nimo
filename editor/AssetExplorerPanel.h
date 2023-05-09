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
        , EditorPanel("Asset Explorer")
        , folderIcon(std::make_shared<nimo::Texture>("icon_folder.png"))
        , fileIcon(std::make_shared<nimo::Texture>("icon_file.png"))
    {
        AssetExplorerPanel::mapExtensionIcon[".nshader"] = std::make_shared<nimo::Texture>("icon_polaroid.png");
        AssetExplorerPanel::mapExtensionIcon[".nmat"] = std::make_shared<nimo::Texture>("icon_paintroll.png");
        AssetExplorerPanel::mapExtensionIcon[".nscene"] = std::make_shared<nimo::Texture>("icon_castle.png");
        AssetExplorerPanel::mapExtensionIcon[".txt"] = std::make_shared<nimo::Texture>("icon_file_alt.png");
        AssetExplorerPanel::mapExtensionIcon[".png"] = std::make_shared<nimo::Texture>("icon_camera.png");
        AssetExplorerPanel::mapExtensionIcon[".jpg"] = std::make_shared<nimo::Texture>("icon_camera.png");
        AssetExplorerPanel::mapExtensionIcon[".obj"] = std::make_shared<nimo::Texture>("icon_cube.png");
        AssetExplorerPanel::mapExtensionIcon[".fbx"] = std::make_shared<nimo::Texture>("icon_cube.png");
        AssetExplorerPanel::mapExtensionIcon[".gltf"] = std::make_shared<nimo::Texture>("icon_cube.png");
        AssetExplorerPanel::mapExtensionIcon[".nprefab"] = std::make_shared<nimo::Texture>("icon_box.png");
        AssetExplorerPanel::mapExtensionIcon[".ogg"] = std::make_shared<nimo::Texture>("icon_audio_wave.png");
        AssetExplorerPanel::mapExtensionIcon[".mp3"] = std::make_shared<nimo::Texture>("icon_audio_wave.png");
        AssetExplorerPanel::mapExtensionIcon[".wav"] = std::make_shared<nimo::Texture>("icon_audio_wave.png");
        AssetExplorerPanel::mapExtensionIcon[".lua"] = std::make_shared<nimo::Texture>("icon_gears.png");
    }
    ~AssetExplorerPanel()
    {
        AssetExplorerPanel::mapExtensionIcon.clear();
    }
    void PaintDirectory(const std::filesystem::path& path);
private:
    void OnRender();
    EditorLayer* m_editor;
    std::shared_ptr<nimo::Texture> folderIcon;
    std::shared_ptr<nimo::Texture> fileIcon;
    std::filesystem::path selectedPath;
    inline static std::unordered_map<std::string, std::shared_ptr<nimo::Texture>> mapExtensionIcon = {};
};
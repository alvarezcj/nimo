#pragma once
#include "EditorPanel.h"

#include <memory>

class EditorLayer;
struct SceneContentsPanel : public EditorPanel
{
    SceneContentsPanel(EditorLayer* editor) : m_editor(editor), EditorPanel("Scene"){}

private:
    void OnRender();
    EditorLayer* m_editor;
};
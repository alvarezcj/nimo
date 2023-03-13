#pragma once
#include "EditorPanel.h"

#include <memory>

class EditorLayer;
struct SceneViewPanel : public EditorPanel
{
    SceneViewPanel(EditorLayer* editor) : m_editor(editor), EditorPanel("Scene View"){}

private:
    void OnRender();
    EditorLayer* m_editor;
};
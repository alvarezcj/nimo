#include "GameViewPanel.h"
#include "imgui.h"
#include "EditorLayer.h"

void GameViewPanel::OnRender()
{
    // Using a Child allow to fill all the space of the window.
    // It also alows customization
    ImGui::BeginChild("GameRender");
    // Get the size of the child (i.e. the whole draw size of the windows).
    ImVec2 wsize = ImGui::GetWindowSize();
    // Because I use the texture from OpenGL, I need to invert the V from the UV.
    ImGui::Image((ImTextureID)m_editor->fb->GetColorAttachmentId(0), wsize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::EndChild();
}

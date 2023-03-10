#include "EditorPanel.h"

#include "imgui.h"

void EditorPanel::Render(){
    if(open){
        ImGui::Begin(m_name.c_str(), &open, ImGuiWindowFlags_NoCollapse);
        OnRender();
        ImGui::End();
    }
}
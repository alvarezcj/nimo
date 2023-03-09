#pragma once
#include "EditorPanel.h"
#include "imgui.h"

struct StatisticsPanel : public EditorPanel
{
    StatisticsPanel() : EditorPanel("Statistics"){}

private:
    void OnRender(){
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
};
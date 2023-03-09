#pragma once
#include "EditorPanel.h"
#include "imgui.h"

struct LogPanel : public EditorPanel
{
    LogPanel() : EditorPanel("Log"){}

    void AddLog(const std::string& str)
    {
        logBuffer.append(str.c_str());
    }

private:
    void OnRender(){
        ImGui::TextUnformatted(logBuffer.begin(), logBuffer.end());
    }
    ImGuiTextBuffer logBuffer;
};
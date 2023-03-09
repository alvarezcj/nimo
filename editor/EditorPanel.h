#pragma once

#include <string>

class EditorPanel 
{
public:
    EditorPanel() = delete;
    EditorPanel(const std::string& name) : m_name(name), open(true){}
    virtual ~EditorPanel(){}
    void Render();
    bool open;

protected:
    virtual void OnRender() = 0;

    std::string m_name;
};
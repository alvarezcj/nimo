#include "Application.h"

#include "Log.h"
#include "glfw/glfw3.h"

nimo::Application::Application()
{
    NIMO_INFO("Created nimo::Application");
    w = new Window();
}
void nimo::Application::Run()
{
    NIMO_INFO("nimo::App run");
    OnStart();
    while(!w->ShouldClose())
    {
        w->ProcessEvents();
        OnUpdate(m_frameTime);
        for(auto l : m_layers)
            l->OnUpdate(m_frameTime);
        w->SwapBuffers();
        float time = Time();
        m_frameTime = time - m_lastFrameTime;
        m_lastFrameTime = time;
    }
    OnClose();
}
nimo::Application::~Application()
{
    delete w;
    NIMO_INFO("Destroyed nimo::Application");    
}

void nimo::Application::AddLayer(Layer* layer)
{
    m_layers.push_back(layer);
    layer->OnAttach();
}
void nimo::Application::RemoveLayer(Layer* layer)
{
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end())
    {
        m_layers.erase(it);
    }
    layer->OnDetach();
}

float nimo::Application::Time()
{
    return (float)glfwGetTime();
}

#include "Application.h"

#include "Log.h"

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
        for(auto l : m_layers)
            l->OnUpdate();
        w->SwapBuffers();
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
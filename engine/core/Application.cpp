#include "Application.h"

#include "Log.h"
#include "glfw/glfw3.h"
#include "core/EventManager.h"

nimo::Application::Application()
    : m_appRunning(true)
{
    NIMO_INFO("Created nimo::Application");
    EventManager::Subscribe(this, &Application::OnWindowClose);
    EventManager::Subscribe(this, &Application::OnWindowResize);
    WindowDescription windowDesc;
    windowDesc.title = "Nimo - Default Application";
    windowDesc.width = 1920;
    windowDesc.height = 1080;
    windowDesc.fullscreen = false;
    windowDesc.decorated = true;
    windowDesc.vsync = true;
    m_window =  std::make_unique<Window>(windowDesc);
}
void nimo::Application::Run()
{
    NIMO_INFO("nimo::App run");
    OnStart();
    while(m_appRunning)
    {
        m_window->ProcessEvents();
        OnUpdate(m_frameTime);
        for(auto l : m_layers)
            l->OnUpdate(m_frameTime);
        m_window->SwapBuffers();
        float time = Time();
        m_frameTime = time - m_lastFrameTime;
        m_lastFrameTime = time;
    }
    OnClose();
}
void nimo::Application::Close()
{
    m_appRunning = false;
    NIMO_INFO("nimo::Application::Close");
}
nimo::Application::~Application()
{
    EventManager::Unsubscribe(this, &Application::OnWindowClose);
    EventManager::Unsubscribe(this, &Application::OnWindowResize);
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

void nimo::Application::OnWindowResize(const WindowResizeEvent& e)
{
    NIMO_INFO("Window resized to {},{}", e.width, e.height);
}
void nimo::Application::OnWindowClose(const WindowCloseEvent& e)
{
    NIMO_INFO("Window closed");
    Close();
}

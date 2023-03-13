#pragma once
#include <vector>
#include "Window.h"
#include "Layer.h"
#include <memory>
#include "WindowEvents.h"

namespace nimo
{
    class __declspec(dllexport) Application{
    public:
        Application();
        virtual ~Application();

        void Run();
        void Close();

        static inline Application& Instance(){ return *instance;}

        virtual void OnStart(){}
        virtual void OnUpdate(float deltaTime){}
        virtual void OnClose(){}

        float Time();
		inline Window& GetWindow() { return *m_window; }

        void AddLayer(Layer* layer);
        void RemoveLayer(Layer* layer);
    private:
        void OnWindowResize(const WindowResizeEvent& e);
        void OnWindowClose(const WindowCloseEvent& e);
        void OnWindowMinimize();
        std::unique_ptr<Window> m_window;
        std::vector<Layer*> m_layers;
        float m_frameTime;
        float m_lastFrameTime;
        bool m_appRunning;

        static Application* instance;
    };
}
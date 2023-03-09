#pragma once
#include <vector>
#include "Window.h"
#include "Layer.h"

namespace nimo
{
    class __declspec(dllexport) Application{
    public:
        Application();
        virtual ~Application();
        void Run();

        virtual void OnStart(){}
        virtual void OnUpdate(float deltaTime){}
        virtual void OnClose(){}
        
        Window* w;
        void AddLayer(Layer* layer);
        void RemoveLayer(Layer* layer);
    private:
        std::vector<Layer*> m_layers;
    };
}
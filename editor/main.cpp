#include "core/Entry.h"
#include <iostream>
#include "EditorLayer.h"


class MyApp : public nimo::Application
{
public:
    MyApp()
        : Application()
    {
        NIMO_DEBUG( "My app constructor");
        editorLayer = new EditorLayer();
        AddLayer(editorLayer);
    }

    ~MyApp()
    {
        NIMO_DEBUG( "My app destructor");
        delete editorLayer;
    }
    EditorLayer* editorLayer;
};

nimo::Application* CreateApplication()
{
    return new MyApp();
}
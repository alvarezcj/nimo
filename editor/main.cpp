#include "core/Entry.h"
#include <iostream>
#include "EditorLayer.h"


class MyApp : public nimo::Application
{
public:
    MyApp(const nimo::ApplicationDescription& desc)
        : Application(desc)
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
    nimo::ApplicationDescription appDesc;
    appDesc.title = "Nimo Editor";
    return new MyApp(appDesc);
}
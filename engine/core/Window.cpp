#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "core/Log.h"

struct nimo::Window::impl{
    WindowDescription description;
    GLFWwindow* handle;
};

nimo::Window::Window(const WindowDescription& description)
    : pimpl(new impl())
{
    pimpl->description = description;
    if (!glfwInit())
    {
        NIMO_CRITICAL("Failed to initalize glfw3");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    pimpl->handle = glfwCreateWindow(description.width, description.height, description.title.c_str(), NULL, NULL);
    if (!pimpl->handle)
    {
        NIMO_CRITICAL("Window or OpenGL context creation failed");
        glfwTerminate();
    }
    glfwMakeContextCurrent(pimpl->handle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        NIMO_CRITICAL("Failed to initialize OpenGL context");
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    // //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(pimpl->handle, true);
    ImGui_ImplOpenGL3_Init("#version 130");


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
}

nimo::Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(pimpl->handle);
}

bool nimo::Window::ShouldClose() 
{
    return glfwWindowShouldClose(pimpl->handle);
}
void nimo::Window::ProcessEvents() 
{
    glfwPollEvents();
}
void nimo::Window::SwapBuffers() 
{
    glfwSwapBuffers(pimpl->handle);
}

void* nimo::Window::GetNativeHandle() const
{
    return pimpl->handle;
}
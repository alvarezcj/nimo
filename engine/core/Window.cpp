#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "core/Log.h"
#include "events/EventManager.h"
#include "events/WindowEvents.h"
#include "events/KeyEvents.h"
#include "events/MouseEvents.h"

struct nimo::Window::impl{
    WindowDescription description;
    GLFWwindow* handle;
    int width;
    int height;
    std::string title;
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
    glfwWindowHint(GLFW_DECORATED, description.decorated);
    
    if(description.fullscreen)
    {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

        glfwWindowHint(GLFW_DECORATED, false);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        pimpl->handle = glfwCreateWindow(mode->width, mode->height, description.title.c_str(), primaryMonitor, nullptr);
    }
    else
    {
        pimpl->handle = glfwCreateWindow(description.width, description.height, description.title.c_str(), NULL, NULL);
    }
    if (!pimpl->handle)
    {
        NIMO_CRITICAL("Window or OpenGL context creation failed");
        glfwTerminate();
    }
    glfwMakeContextCurrent(pimpl->handle);
    glfwMakeContextCurrent(pimpl->handle);
    glfwSetWindowUserPointer(pimpl->handle, reinterpret_cast<void*>(this));
    
    glfwSetWindowSizeCallback(pimpl->handle, [](GLFWwindow* window, int width, int height)
    {
        auto data = ((Window*)glfwGetWindowUserPointer(window));
        data->pimpl->width = width;
        data->pimpl->height = height;
        EventManager::Publish(WindowResizeEvent(width,height));
    });
    glfwSetWindowCloseCallback(pimpl->handle, [](GLFWwindow* window)
    {
        auto data = ((Window*)glfwGetWindowUserPointer(window));
        EventManager::Publish(WindowCloseEvent());
    });
    glfwSetWindowIconifyCallback(pimpl->handle, [](GLFWwindow* window, int iconified)
    {
        EventManager::Publish(WindowMinimizedEvent());
    });

    glfwSetKeyCallback(pimpl->handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:
        {
            EventManager::Publish(KeyPressedEvent(key));
            break;
        }
        case GLFW_RELEASE:
        {
            EventManager::Publish(KeyReleasedEvent(key));
            break;
        }
        case GLFW_REPEAT:
        {
            EventManager::Publish(KeyPressedEvent(key));
            break;
        }
        }
    });

    // glfwSetCharCallback(pimpl->handle, [](GLFWwindow* window, uint32_t codepoint)
    //     {
    //         auto data = ((Window*)glfwGetWindowUserPointer(window));
    //         NIMO_DEBUG("glfwSetCharCallback");
    //     });

    glfwSetMouseButtonCallback(pimpl->handle, [](GLFWwindow* window, int button, int action, int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:
        {
            EventManager::Publish(MouseButtonPressedEvent(button));
            break;
        }
        case GLFW_RELEASE:
        {
            EventManager::Publish(MouseButtonReleasedEvent(button));
            break;
        }
        }
    });

    glfwSetScrollCallback(pimpl->handle, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        auto data = ((Window*)glfwGetWindowUserPointer(window));
        EventManager::Publish(MouseScrollEvent(xOffset, yOffset));
    });
    glfwSetCursorPosCallback(pimpl->handle, [](GLFWwindow* window, double x, double y)
    {
        auto data = ((Window*)glfwGetWindowUserPointer(window));
        EventManager::Publish(MousePositionEvent(x, y));
    });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        NIMO_CRITICAL("Failed to initialize OpenGL context");
    }

    // Update window size to actual size
    {
        int width, height;
        glfwGetWindowSize(pimpl->handle, &width, &height);
        pimpl->width = width;
        pimpl->height = height;
    }

    // // Set icon
    // {
    //     GLFWimage icon;
    //     int channels;
    //     icon.pixels = stbi_load("Resources/Editor/H_logo_square.png", &icon.width, &icon.height, &channels, 4);
    //     glfwSetWindowIcon(m_Window, 1, &icon);
    //     stbi_image_free(icon.pixels);
    // }

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

void nimo::Window::VSync(bool enabled)
{
    glfwSwapInterval(enabled);
    pimpl->description.vsync;
}
bool nimo::Window::VSync() const
{
    return pimpl->description.vsync;
}
void nimo::Window::SetResizable(bool resizable) const
{
    glfwSetWindowAttrib(pimpl->handle, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
}
void nimo::Window::Maximize()
{
    glfwMaximizeWindow(pimpl->handle);
}
void nimo::Window::CenterWindow()
{
    const GLFWvidmode* videmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int x = (videmode->width / 2) - (pimpl->width / 2);
    int y = (videmode->height / 2) - (pimpl->height / 2);
    glfwSetWindowPos(pimpl->handle, x, y);
}
const std::string& nimo::Window::GetTitle() const
{
    return pimpl->title;
}
void nimo::Window::SetTitle(const std::string& title)
{
    pimpl->title = title;
    glfwSetWindowTitle(pimpl->handle, pimpl->title.c_str());
}
unsigned int nimo::Window::GetWidth() const
{
    return pimpl->width;
}
unsigned int nimo::Window::GetHeight() const
{
    return pimpl->height;
}
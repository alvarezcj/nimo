#include "Input.h"
#include "glfw/glfw3.h"
#include "core/Window.h"
#include "core/Application.h"

bool nimo::Input::GetMouseButton(MouseButton button)
{
    Window& window = static_cast<Window&>(Application::Instance().GetWindow());
    int state = glfwGetMouseButton(static_cast<GLFWwindow*>(window.GetNativeHandle()), static_cast<int32_t>(button));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
float nimo::Input::GetMouseX()
{
    return GetMousePosition().first;
}
float nimo::Input::GetMouseY()
{
    return GetMousePosition().second;
}
std::pair<float, float> nimo::Input::GetMousePosition()
{

    Window& window = static_cast<Window&>(Application::Instance().GetWindow());
    double x, y;
    glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetNativeHandle()), &x, &y);
    return { (float)x, (float)y };
}
bool nimo::Input::GetKey(KeyCode keycode)
{
    Window& window = static_cast<Window&>(Application::Instance().GetWindow());
    int state = glfwGetKey(static_cast<GLFWwindow*>(window.GetNativeHandle()), static_cast<int32_t>(keycode));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
void nimo::Input::SetCursorMode(CursorMode mode)
{
    Window& window = static_cast<Window&>(Application::Instance().GetWindow());
    glfwSetInputMode(static_cast<GLFWwindow*>(window.GetNativeHandle()), GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
}
nimo::CursorMode nimo::Input::GetCursorMode()
{
    Window& window = static_cast<Window&>(Application::Instance().GetWindow());
    return (CursorMode)(glfwGetInputMode(static_cast<GLFWwindow*>(window.GetNativeHandle()), GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
}

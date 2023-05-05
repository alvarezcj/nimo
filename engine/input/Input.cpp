#include "Input.h"
#include "glfw/glfw3.h"
#include "core/Window.h"
#include "core/Application.h"
#include "events/EventManager.h"

nimo::Input* nimo::Input::instance = nullptr;

void nimo::Input::Initialize()
{
    if(!instance)
        instance = new Input();
    instance->pressedMouseButtons.reserve(3);
    instance->releasedMouseButtons.reserve(3);
    instance->pressedKeys.reserve(10);
    instance->releasedKeys.reserve(10);
    EventManager::Subscribe(instance, &OnKeyPressed);
    EventManager::Subscribe(instance, &OnKeyReleased);
    EventManager::Subscribe(instance, &OnMouseButtonPressed);
    EventManager::Subscribe(instance, &OnMouseButtonReleased);
    EventManager::Subscribe(instance, &OnMouseScroll);
}
void nimo::Input::Cleanup()
{
    EventManager::Unsubscribe(instance, &OnKeyPressed);
    EventManager::Unsubscribe(instance, &OnKeyReleased);
    EventManager::Unsubscribe(instance, &OnMouseButtonPressed);
    EventManager::Unsubscribe(instance, &OnMouseButtonReleased);
    EventManager::Unsubscribe(instance, &OnMouseScroll);
    delete instance;
}
void nimo::Input::Update()
{
    instance->pressedMouseButtons.clear();
    instance->releasedMouseButtons.clear();
    instance->pressedKeys.clear();
    instance->releasedKeys.clear();
    instance->mouseScroll = {0.0, 0.0};
}
bool nimo::Input::GetMouseButton(MouseButton button)
{
    Window& window = static_cast<Window&>(Application::Instance().GetWindow());
    int state = glfwGetMouseButton(static_cast<GLFWwindow*>(window.GetNativeHandle()), static_cast<int32_t>(button));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool nimo::Input::GetMouseButtonPressed(MouseButton button)
{
    return std::find(instance->pressedMouseButtons.begin(), instance->pressedMouseButtons.end(), button) != instance->pressedMouseButtons.end();
}
bool nimo::Input::GetMouseButtonReleased(MouseButton button)
{
    return std::find(instance->releasedMouseButtons.begin(), instance->releasedMouseButtons.end(), button) != instance->releasedMouseButtons.end();
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
std::pair<double, double> nimo::Input::GetMouseScroll()
{
    return instance->mouseScroll;
}
bool nimo::Input::GetKey(KeyCode keycode)
{
    Window& window = static_cast<Window&>(Application::Instance().GetWindow());
    int state = glfwGetKey(static_cast<GLFWwindow*>(window.GetNativeHandle()), static_cast<int32_t>(keycode));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool nimo::Input::GetKeyPressed(KeyCode k)
{
    return std::find(instance->pressedKeys.begin(), instance->pressedKeys.end(), k) != instance->pressedKeys.end();
}
bool nimo::Input::GetKeyReleased(KeyCode k)
{
    return std::find(instance->releasedKeys.begin(), instance->releasedKeys.end(), k) != instance->releasedKeys.end();
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

void nimo::Input::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
{
    pressedMouseButtons.push_back((MouseButton)e.button);
}
void nimo::Input::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
{
    releasedMouseButtons.push_back((MouseButton)e.button);
}
void nimo::Input::OnMouseScroll(const MouseScrollEvent& e)
{
    mouseScroll = {e.xOffset, e.yOffset};
}
void nimo::Input::OnKeyPressed(const KeyPressedEvent& e)
{
    pressedKeys.push_back((KeyCode)e.key);
}
void nimo::Input::OnKeyReleased(const KeyReleasedEvent& e)
{
    releasedKeys.push_back((KeyCode)e.key);
}

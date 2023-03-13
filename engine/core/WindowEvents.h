#include "Event.h"

namespace nimo{
struct WindowResizeEvent: public Event
{
    WindowResizeEvent(int width, int height): width(width), height(height){};
    int width;
    int height;
};
struct WindowCloseEvent: public Event{};
struct WindowMinimizedEvent: public Event{};

struct MouseButtonPressedEvent: public Event
{
    MouseButtonPressedEvent(int button): button(button){};
    int button;
};
struct MouseButtonReleasedEvent: public Event
{
    MouseButtonReleasedEvent(int button): button(button){};
    int button;
};
struct MousePositionEvent: public Event
{
    MousePositionEvent(double xPos, double yPos):xPos(xPos), yPos(yPos){};
    double xPos;
    double yPos;
};
struct MouseScrollEvent: public Event
{
    MouseScrollEvent(double xOffset, double yOffset): xOffset(xOffset), yOffset(yOffset) {};
    double xOffset;
    double yOffset;
};
struct KeyPressedEvent: public Event
{
    KeyPressedEvent(int key): key(key){};
    int key;
};
struct KeyReleasedEvent: public Event
{
    KeyReleasedEvent(int key): key(key){};
    int key;
};
};
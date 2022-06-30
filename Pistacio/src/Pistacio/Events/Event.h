#pragma once

#include "pch.h"
#include "Pistacio/Core.h"
#include <spdlog/fmt/bundled/format.h>

namespace Pistacio {
  
  enum class EventType
  {
    WindowCloseEvent,
    WindowResizeEvent,
    KeyEvent,
    MouseMoveEvent,
    MouseClickEvent
  };
  
  class Event
  {
  public:
    virtual ~Event() = default;
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
  
    bool Handled = false;
  
    inline std::ostream& operator<<(std::ostream& os)
    {
      return os << GetName();
    }
  };

  class WindowCloseEvent : public Event
  {
  public:
    WindowCloseEvent() = default;
    virtual ~WindowCloseEvent() = default;
    virtual EventType GetEventType() const override;
    virtual const char* GetName() const override;
  };
  
  class WindowResizeEvent : public Event
  {
  public:
    WindowResizeEvent(int width, int height) : width(width), height(height) { }
    WindowResizeEvent(WindowResizeEvent& other) : width(other.width), height(other.height) { }
    virtual ~WindowResizeEvent() { }
    virtual EventType GetEventType() const override;
    virtual const char* GetName() const override;
    const int width;
    const int height;
  };

  class KeyEvent : public Event
  {
  public:
    KeyEvent(Input::ButtonAction action, Input::KeyCode key, int modFlags) : action(action), key(key), modFlags(modFlags) { }
    KeyEvent(KeyEvent& other) : action(other.action), key(other.key), modFlags(other.modFlags) { }
    virtual ~KeyEvent() = default;
    virtual EventType GetEventType() const override;
    virtual const char* GetName() const override;
    const Input::ButtonAction action;
    const Input::KeyCode key;
    const int modFlags;
  };

  class MouseMoveEvent : public Event
  {
  public:
    MouseMoveEvent(double x, double y) : x(x), y(y) { }
    MouseMoveEvent(MouseMoveEvent& other) : x(other.x), y(other.y) { }
    virtual ~MouseMoveEvent() { }
    virtual EventType GetEventType() const override;
    virtual const char* GetName() const override;
    const double x;
    const double y;
  };

  class MouseClickEvent : public Event
  {
  public:
    MouseClickEvent(Input::ButtonAction action, Input::MouseCode mouseKey, int modFlags) : action(action), mouseKey(mouseKey), modFlags(modFlags) { }
    MouseClickEvent(MouseClickEvent& other) : action(other.action), mouseKey(other.mouseKey), modFlags(other.modFlags) { }
    virtual ~MouseClickEvent() = default;
    virtual EventType GetEventType() const override;
    virtual const char* GetName() const override;
    const Input::ButtonAction action;
    const Input::MouseCode mouseKey;
    const int modFlags;
  };

}






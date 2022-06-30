
#include "pch.h"
#include "Event.h"

namespace Pistacio {

  EventType WindowCloseEvent::GetEventType() const
  {
    return EventType::WindowCloseEvent;
  }
  
  const char* WindowCloseEvent::GetName() const
  {
    return "WindowCloseEvent";
  }
  
  EventType WindowResizeEvent::GetEventType() const
  {
    return EventType::WindowResizeEvent;
  }
  
  const char* WindowResizeEvent::GetName() const
  {
    return "WindowResizeEvent";
  }

  EventType KeyEvent::GetEventType() const
  {
    return EventType::KeyEvent;
  }

  const char* KeyEvent::GetName() const
  {
    return "KeyEvent";
  }

  EventType MouseMoveEvent::GetEventType() const
  {
    return EventType::MouseMoveEvent;
  }

  const char* MouseMoveEvent::GetName() const
  {
    return "MouseMoveEvent";
  }

  EventType MouseClickEvent::GetEventType() const
  {
    return EventType::MouseClickEvent;
  }

  const char* MouseClickEvent::GetName() const
  {
    return "MouseClickEvent";
  }

}

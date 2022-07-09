#pragma once

#include "pch.h"


namespace Pistacio
{

  class Window
  {
  public:
    
    Window() = default;
    virtual ~Window() = default;

    virtual void Present() = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;
    virtual void Shutdown() = 0;

    static std::unique_ptr<Window> Create(std::string windowName, uint32_t width, uint32_t height, bool hintFloat);

  };

  struct WindowCloseEvent { };

  struct WindowResizeEvent
  {
    const int width;
    const int height;
  };

  struct KeyEvent
  {
    const Input::ButtonAction action;
    const Input::KeyCode key;
    const int modFlags;
  };

  struct MouseMoveEvent
  {
    const double x;
    const double y;
  };

  struct MouseClickEvent
  {
    const Input::ButtonAction action;
    const Input::MouseCode mouseKey;
    const int modFlags;
  };
}


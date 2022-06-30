#pragma once

#include "pch.h"
#include "Pistacio/Core.h"



namespace Pistacio
{

  class Application;

  class Window
  {
  public:
    
    struct WindowProperties
    {
      std::string Title;
      uint32_t Width;
      uint32_t Height;

      WindowProperties(const std::string& title = "Pistacio", uint32_t width = 1280, uint32_t height = 720) : Title(title), Width(width), Height(height) { }
    };

    Window() = default;
    virtual ~Window() = default;

    virtual void OnUpdate() = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;
    virtual void Shutdown() = 0;

    virtual void SetEventCallback(const std::function<void(Event&)> callback) = 0;

    static std::unique_ptr<Window> Create(const WindowProperties& props = WindowProperties());

  };
}


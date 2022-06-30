#pragma once
#include "pch.h"
#include "Pistacio/Window.h"

#include "GLFW/glfw3.h"

namespace Pistacio
{
  class WindowsWindow : public Window
  {

  public:
    WindowsWindow(const WindowProperties& props);
    virtual ~WindowsWindow() = default;

    void OnUpdate() override;

    inline uint32_t GetWidth() const override { return data.Width; };
    inline uint32_t GetHeight() const override { return data.Height; };

    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    void Shutdown();

    void SetEventCallback(const std::function<void(Event&)> callback) override;

  private:
    void Init(const WindowProperties& props);

    struct WindowData {
      std::string Title;
      uint32_t Width, Height;
      bool VSync;

      std::function<void(Event&)> EventCallback;
    };

    WindowData data;
    GLFWwindow* glfwWindow = nullptr;
    

  };
}



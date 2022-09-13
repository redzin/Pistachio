#pragma once

#include "pch.h"
#include "Pistacio/Rendering/Device.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Pistacio
{

  class Window
  {
    friend class Application;

  public:
    Window(std::string windowName, uint32_t m_Width, uint32_t m_Height, EventLibrary& eventLib);
    ~Window() = default;
    void HideCursor();
    void ShowCursor();
    bool IsKeyPressed(Input::KeyCode code) const;
    bool IsMouseButtonPressed(Input::MouseCode code) const;
    double GetMouseX() const;
    double GetMouseY() const;
    double GetScrollX() const;
    double GetScrollY() const;
    glm::dvec2 GetMousePos() const;
    void MakeRenderingContextCurrent() const;
    RenderingAPI::GetRenderAPIProcAddress GetProcAddress() const;

    inline uint32_t GetWidth() const { return m_Width; };
    inline uint32_t GetHeight() const { return m_Height; };

    void SetVSync(bool enabled);
    bool IsVSync() const;

    Device& GetDevice() { return *m_Device; }

  private:
    void PollUIEvents();
    void SwapBuffers() const;
    void Shutdown();

  private:
    void Init(bool hintFloat);

    std::string m_Title;
    uint32_t m_Width, m_Height;
    bool m_VSync;

    struct CallbackData {
      double ScrollX = 0, ScrollY = 0;
      EventLibrary& eventLib;
    };
    CallbackData m_CallbackData;

    GLFWwindow* m_GLFWWindow = nullptr;

    Ref<Device> m_Device = nullptr;

  };

  struct WindowCloseEvent { };

  struct WindowResizeEvent
  {
    const int m_Width;
    const int m_Height;
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

  struct MouseScrollEvent
  {
    double xoffset, yoffset;
  };

  struct MouseClickEvent
  {
    const Input::ButtonAction action;
    const Input::MouseCode mouseKey;
    const int modFlags;
    int x, y;
  };
}


#pragma once
#include "pch.h"
#include "Pistacio/Core/Window.h"
#include "Pistacio/Core/Application.h"

#include "Pistacio/Rendering/Swapchain.h"

namespace Pistacio
{
  class WindowsWindow : public Window
  {

  public:
    WindowsWindow(std::string windowName, uint32_t width, uint32_t height, bool hintFloat);
    virtual ~WindowsWindow() = default;

    void HideCursor() override;
    void ShowCursor() override;
    bool IsKeyPressed(Input::KeyCode code) const override;
    bool IsMouseButtonPressed(Input::MouseCode code) const override;
    double GetMouseX() const override;
    double GetMouseY() const override;
    glm::dvec2 GetMousePos() const override;

    inline uint32_t GetWidth() const override { return width; };
    inline uint32_t GetHeight() const override { return height; };

    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

  private:
    void PollUIEvents() override;
    void Present() override;

    void Shutdown() override;

  private:
    void Init(std::string windowName, uint32_t width, uint32_t height, bool hintFloat);


    std::string title;
    uint32_t width, height;
    bool vSync;

    GLFWwindow* glfwWindow = nullptr;

    Swapchain* swapchain;

  };
}



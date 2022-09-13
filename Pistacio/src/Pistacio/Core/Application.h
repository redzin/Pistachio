#pragma once

#include "pch.h"
#include "Core.h"

using namespace std::literals::chrono_literals;

namespace Pistacio {
  

  class Application
  {
  public:
    Application(std::string appName);
    Application(std::string appName, bool WindowHintFloat);
    Application(std::string appName, uint32_t m_Width, uint32_t m_Height, bool WindowHintFloat);
    Application(std::string appName, uint32_t m_Width, uint32_t m_Height);
    Application(Application& app) = default;
    ~Application();

  private:
    static bool m_Instantiated;

  public:
    void Run();
    void AddLayer(Layer* layer);
    void AddOverlay(Layer* layer);

  private:
    EventConsumed OnWindowCloseEvent();
    EventConsumed OnWindowResizeEvent(int m_Width, int m_Height);

  private:
    bool m_AppRunning = true;

  private:
    EventLibrary m_EventLibrary;
    Window m_Window;
    LayerStack m_LayerStack;
    Scope<ImGuiRenderer> m_ImguiRenderer;

  private:
    std::chrono::steady_clock::duration lastFrameTime = 6ms;
  };

  class ApplicationFactory
  {
  public:
    inline static std::unique_ptr<Application> Create()
    {
      return std::unique_ptr<Application>(doCreate());
    }

  private:
    static Application* doCreate();
  };

}


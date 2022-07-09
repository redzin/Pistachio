#pragma once

#include "pch.h"
#include "Core.h"

#ifdef PSTC_PLATFORM_WINDOWS
#include "PistacioPlatform/Windows/WindowsWindow.h"
#endif

namespace Pistacio {
  
  class PSTC_API Application
  {
  public:
    Application(std::string appName);
    Application(std::string appName, bool WindowHintFloat);
    Application(std::string appName, uint32_t width, uint32_t height, bool WindowHintFloat);
    Application(Application& app) = default;
    ~Application();

    void Run();
    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    static Application* Get();
    EventLibrary& GetEventLibrary();

  private:
    EventLibrary EventLibrary;
    bool OnWindowCloseEvent();
    bool OnWindowResizeEvent(int width, int height);

    bool app_running = true;
    std::unique_ptr<Window> window;
    std::unique_ptr<LayerStack> layerStack;
    std::unique_ptr<ImGuiRenderer> imguiRenderer;

    static Application* singletonInstance;
  };

  class ApplicationFactory
  {
  public:
    inline static std::unique_ptr<Application> Create()
    {
      return std::unique_ptr<Application>(doCreate());
    }

  private:
    static PSTC_API Application* doCreate();
  };

}


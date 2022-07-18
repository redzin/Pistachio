#pragma once

#include "pch.h"
#include "Core.h"

#include "Pistacio/Rendering/Camera/Camera.h"

#ifdef PSTC_PLATFORM_WINDOWS
#include "PistacioPlatform/Windows/WindowsWindow.h"
#endif

using namespace std::literals::chrono_literals;

namespace Pistacio {
  
  class Application
  {
  public:
    Application(std::string appName);
    Application(std::string appName, bool WindowHintFloat);
    Application(std::string appName, uint32_t width, uint32_t height, bool WindowHintFloat);
    Application(Application& app) = default;
    ~Application();

    void Run();
    void AddLayer(Layer* layer);
    void AddOverlay(Layer* layer);
    EventLibrary& GetEventLibrary();
    Window& GetWindow();

    static Application* Get();

  private:
    EventLibrary EventLibrary;
    bool OnWindowCloseEvent();
    bool OnWindowResizeEvent(int width, int height);

    bool app_running = true;
    std::unique_ptr<Window> window;
    std::unique_ptr<LayerStack> layerStack;
    std::unique_ptr<ImGuiRenderer> imguiRenderer;

    std::chrono::steady_clock::duration lastFrameTime = 6ms;

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
    static Application* doCreate();
  };

  struct AppUpdateEvent
  {
    std::chrono::steady_clock::duration dt;
  };

}


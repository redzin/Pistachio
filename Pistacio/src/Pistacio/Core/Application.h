#pragma once

#include "pch.h"
#include "Core.h"

namespace Pistacio {
  
  class PSTC_API Application
  {
  public:
    Application(std::string appName);
    Application(std::string appName, uint32_t width, uint32_t height);
    Application(Application& app) = default;
    ~Application() { }

    void OnEvent(Event& e);
    void Run();
    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    static Application* Get();

  private:
    bool app_running = true;
    std::unique_ptr<Window> window;
    LayerStack layerStack;
    ImGuiRenderer imguiRenderer;

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


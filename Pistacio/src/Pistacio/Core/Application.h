#pragma once

#include "pch.h"
#include "Core.h"

namespace Pistacio {
  
  class PSTC_API Application
  {
  public:
    Application();
    Application(Application& app) = default;
    ~Application() { }

    void OnEvent(Event& e);
    void Init();
    void Run();
    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

  private:
    bool app_running = true;
    std::unique_ptr<Window> window;
    LayerStack layerStack;
    ImGuiRenderer imguiRenderer;
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


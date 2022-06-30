#pragma once

#include "pch.h"
#include "Core.h"
#include "Window.h"

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

    bool app_running = true;



  private:
    std::unique_ptr<Window> window;
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


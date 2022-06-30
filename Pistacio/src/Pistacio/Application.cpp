
#include "pch.h"
#include "Application.h"
#include "Events/EventRelay.h"

#include <GLFW/glfw3.h>

#ifdef PSTC_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Pistacio
{
  Application::Application()
  {
    INIT_LOGGER
    PSTC_CORE_INFO("Logger initialized!");
    Init();
  }

  void Application::Init() {
    window = Window::Create();
    window->SetEventCallback([this](Event& e) { OnEvent(e); });
  }

  void Application::OnEvent(Event& e)
  {
    switch (e.GetEventType())
    {

    case EventType::WindowCloseEvent:
      app_running = false;
      break;

    case EventType::WindowResizeEvent:
      WindowResizeEvent& re = (WindowResizeEvent&)e;
      break;

    }
  }

  void Application::Run()
  {
    while (app_running)
    {
      glClearColor(1, 0, 1, 1);
      glClear(GL_COLOR_BUFFER_BIT);
      window->OnUpdate();
    }

    window->Shutdown();
  }
}



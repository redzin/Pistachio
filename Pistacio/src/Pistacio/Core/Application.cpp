
#include "pch.h"
#include "Application.h"

#ifdef PSTC_PLATFORM_WINDOWS
#include "Pistacio/Platform/Windows/WindowsWindow.h"
#endif

namespace Pistacio
{
  Application::Application() : imguiRenderer(ImGuiRenderer())
  {
    INIT_LOGGER
    PSTC_CORE_INFO("Logger initialized!");
    Init();
  }

  void Application::Init() {
    window = Window::Create({"CounterApp", 200, 40});
    window->SetEventCallback([this](Event& e) { OnEvent(e); });

    imguiRenderer.Init();

  }
  
  void Application::OnEvent(Event& e)
  {
    switch (e.GetEventType())
    {
      case EventType::WindowCloseEvent:
        PSTC_CORE_INFO("Exiting...");
        app_running = false;
        e.Handled = true;
        break;
      case EventType::WindowResizeEvent:
        #ifdef PSTC_VERBOSE_DEBUG
        WindowResizeEvent resizeEvent = reinterpret_cast<WindowResizeEvent&>(e);
          PSTC_CORE_INFO("Resize to ({0}, {1})", resizeEvent.width, resizeEvent.height);
        #endif
        break;
    }

    for (auto iter = layerStack.rbegin(); iter < layerStack.rend(); iter++)
    {
      if (e.Handled) break;
      static_cast<Layer*>(*iter)->OnEvent(e);
    }
  }

  void Application::Run()
  {
    while (app_running)
    {

      for (Layer* layer : layerStack)
      {
        layer->OnUpdate();
      }

      imguiRenderer.BeginRender();

      for (Layer* layer : layerStack)
      {
        layer->OnGuiRender();
      }

      imguiRenderer.EndRender();

      window->OnUpdate();

    }

    window->Shutdown();
    imguiRenderer.Cleanup();
  }
  void Application::PushLayer(Layer* layer)
  {
    layerStack.PushLayer(layer);
    layer->OnAttach();
  }
  void Application::PushOverlay(Layer* layer)
  {
    layerStack.PushOverlay(layer);
    layer->OnAttach();
  }
}



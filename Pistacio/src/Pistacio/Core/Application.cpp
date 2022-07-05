
#include "pch.h"
#include "Application.h"

#include "glm/glm.hpp"

#ifdef PSTC_PLATFORM_WINDOWS
#include "Pistacio/Platform/Windows/WindowsWindow.h"
#endif

namespace Pistacio
{
  Application* Application::singletonInstance = nullptr;

  Application::Application(std::string appName, uint32_t width, uint32_t height) : imguiRenderer(ImGuiRenderer())
  {
    PSTC_CORE_ASSERT(!singletonInstance, "Application instance already created, only one instance allowed!");
    singletonInstance = this;

    INIT_LOGGER
      PSTC_CORE_INFO("Logger initialized!");

    window = Window::Create({ appName, width, height });
    window->SetEventCallback([this](Event& e) { OnEvent(e); });

    imguiRenderer.Init();
  }


  Application::Application(std::string appName) : Application(appName, 1080, 720)
  {
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

  Application* Application::Get()
  {
    return singletonInstance;
  }

}


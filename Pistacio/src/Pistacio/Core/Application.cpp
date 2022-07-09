
#include "pch.h"
#include "Application.h"

namespace Pistacio
{

  Application* Application::singletonInstance = nullptr;

  Application::Application(std::string appName, uint32_t width, uint32_t height, bool windowHintFloat)
  {
    PSTC_CORE_ASSERT(!singletonInstance, "Application instance already created, only one instance allowed!");
    singletonInstance = this;

    INIT_LOGGER
      PSTC_CORE_INFO("Logger initialized!");

    window = Window::Create(appName, width, height, windowHintFloat);

    EventLibrary.Subscribe<WindowCloseEvent>([this](WindowCloseEvent& e)
      {
        return OnWindowCloseEvent();
      });
    EventLibrary.Subscribe<WindowResizeEvent>([this](WindowResizeEvent& e)
      {
        return OnWindowResizeEvent(e.width, e.height);
      });
    layerStack.reset(new LayerStack);
    imguiRenderer.reset(ImGuiRenderer::Create());
  }

  Application::~Application()
  {
  }

  Application::Application(std::string appName) : Application(appName, 1080, 720, false)
  {
  }

  Application::Application(std::string appName, bool windowHintFloat) : Application(appName, 1080, 720, windowHintFloat)
  {
  }

  bool Application::OnWindowCloseEvent()
  {
    PSTC_CORE_INFO("Exiting...");
    app_running = false;
    return false;
  }

  bool Application::OnWindowResizeEvent(int width, int height)
  {
    #ifdef PSTC_VERBOSE_DEBUG
      PSTC_CORE_INFO("Resize to ({0}, {1})", width, height);
    #endif
    return true;
  }

  void Application::Run()
  {
    while (app_running)
    {
      for (auto layerIter = layerStack->rbegin(); layerIter != layerStack->rend(); layerIter++)
      {
        (*layerIter)->OnRender();
      }

      imguiRenderer->BeginRender();
      for (Layer* layer : *layerStack)
      {
        layer->OnGuiRender();
      }
      imguiRenderer->EndRender();

      window->Present();

    }

    imguiRenderer.reset(nullptr); // make sure imgui is shutdown before the window
    window->Shutdown();
  }

  void Application::PushLayer(Layer* layer)
  {
    layerStack->PushLayer(layer);
    layer->OnAttach();
  }
  void Application::PushOverlay(Layer* layer)
  {
    layerStack->PushOverlay(layer);
    layer->OnAttach();
  }

  Application* Application::Get()
  {
    return singletonInstance;
  }

  EventLibrary& Application::GetEventLibrary()
  {
    return EventLibrary;
  }

}


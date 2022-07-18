
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
    window->SetVSync(false);

    EventLibrary.Subscribe<WindowCloseEvent>([this](WindowCloseEvent e)
      {
        return OnWindowCloseEvent();
      });
    EventLibrary.Subscribe<WindowResizeEvent>([this](WindowResizeEvent e)
      {
        return OnWindowResizeEvent(e.width, e.height);
      });
    layerStack.reset(new LayerStack);
    imguiRenderer.reset(ImGuiRenderer::Create());
    Renderer::Init();
    EventLibrary.Register<AppUpdateEvent>();

#ifdef PSTC_DEBUG
    EventLibrary.Subscribe<KeyEvent>([](KeyEvent e)
      {
        if (e.key == Input::KeyCode::P && e.action == Input::ButtonAction::KeyPressed)
        {
          PSTC_CORE_INFO("Exeution paused, press Enter to resume...");
          std::cin.get();
        }
        return true;
      });
#endif
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
    Renderer::SetViewport(0,0,width,height);
    return true;
  }

  void Application::Run()
  {
    //Run loop
    while (app_running)
    {

      std::chrono::time_point start = std::chrono::steady_clock::now();

      window->PollUIEvents();

      // Consider refactoring events with ECS according to:
      // https://gamedev.stackexchange.com/questions/141636/event-handling-in-pure-entity-component-systems-is-this-approach-correct
      EventLibrary.Publish(AppUpdateEvent{ lastFrameTime });

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

      lastFrameTime = std::chrono::steady_clock::now() - start;
    }

    imguiRenderer.reset(nullptr); // make sure imgui is shutdown before the window
    window->Shutdown();
  }

  void Application::AddLayer(Layer* layer)
  {
    layerStack->PushLayer(layer);
    layer->OnAttach();
  }
  void Application::AddOverlay(Layer* layer)
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

  Window& Application::GetWindow()
  {
    return *window;
  }

}


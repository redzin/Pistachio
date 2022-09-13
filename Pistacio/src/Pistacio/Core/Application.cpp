
#include "pch.h"
#include "Application.h"


namespace Pistacio
{
  bool Application::m_Instantiated = false;

  Application::Application(std::string appName, uint32_t m_Width, uint32_t m_Height, bool windowHintFloat)
    : m_Window(Window(appName, m_Width, m_Height, m_EventLibrary))
  {

    PSTC_CORE_ASSERT(!m_Instantiated, "Application instance already created, only one instance allowed!");
    m_Instantiated = true;

    INIT_LOGGER();
    PSTC_CORE_INFO("Logger initialized!");

    m_Window.Init(windowHintFloat);
    m_Window.SetVSync(true);
    m_EventLibrary.Subscribe<WindowCloseEvent>([this](WindowCloseEvent e)
      {
        return OnWindowCloseEvent();
      });
    m_EventLibrary.Subscribe<WindowResizeEvent>([this](WindowResizeEvent e)
      {
        return OnWindowResizeEvent(e.m_Width, e.m_Height);
      }
    );

    m_ImguiRenderer = ImGuiRenderer::Create();

#ifdef PSTC_DEBUG
    m_EventLibrary.Subscribe<KeyEvent>([](KeyEvent e)
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

  Application::Application(std::string appName, uint32_t m_Width, uint32_t m_Height) : Application(appName, m_Width, m_Height, false)
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
    m_AppRunning = false;
    return false;
  }

  bool Application::OnWindowResizeEvent(int m_Width, int m_Height)
  {
    #ifdef PSTC_VERBOSE_DEBUG
      PSTC_CORE_INFO("Resize to ({0}, {1})", width, height);
    #endif
    //Renderer::Resize(width,height);
    return true;
  }

  void Application::Run()
  {
    //Run loop
    while (m_AppRunning)
    {

      std::chrono::time_point start = std::chrono::steady_clock::now();

      m_Window.PollUIEvents();

      // Consider refactoring events with ECS according to:
      // https://gamedev.stackexchange.com/questions/141636/event-handling-in-pure-entity-component-systems-is-this-approach-correct

      for (auto layerIter = m_LayerStack.rbegin(); layerIter != m_LayerStack.rend(); layerIter++)
      {
        (*layerIter)->OnUpdate(lastFrameTime);
        (*layerIter)->OnRender(m_Window.GetDevice(), m_Window);
      }

      m_ImguiRenderer->BeginRender();
      for (Layer* layer : m_LayerStack)
      {
        layer->OnGuiRender(m_Window, m_EventLibrary);
      }

      m_ImguiRenderer->EndRender();

      m_Window.SwapBuffers();

      lastFrameTime = std::chrono::steady_clock::now() - start;
    }

    m_ImguiRenderer.reset(nullptr); // make sure imgui is shutdown before the window
    m_Window.Shutdown();
  }

  void Application::AddLayer(Layer* layer)
  {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach(m_Window, m_EventLibrary);
  }
  void Application::AddOverlay(Layer* layer)
  {
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach(m_Window, m_EventLibrary);
  }

}


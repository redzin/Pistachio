#include "pch.h"
#include "Window.h"

namespace Pistachio
{
  static bool GLFW_Initialized = false;

  int ConvertGLFWModToPistachioMod(int mods)
  {
    int modFlags = 0;
    if (mods & GLFW_MOD_SHIFT) modFlags += (int)Input::KeyModFlags::Shift;
    if (mods & GLFW_MOD_CONTROL) modFlags += (int)Input::KeyModFlags::Control;
    if (mods & GLFW_MOD_ALT) modFlags += (int)Input::KeyModFlags::Alt;
    if (mods & GLFW_MOD_SUPER) modFlags += (int)Input::KeyModFlags::Super;
    if (mods & GLFW_MOD_CAPS_LOCK) modFlags += (int)Input::KeyModFlags::CapsLock;
    if (mods & GLFW_MOD_NUM_LOCK) modFlags += (int)Input::KeyModFlags::NumLock;
    return modFlags;
  }

  Window::Window(std::string windowName, uint32_t width, uint32_t height, EventLibrary& eventLib)
  : m_CallbackData(CallbackData{ 0,0, eventLib }),
    m_Title(windowName),
    m_Width(width),
    m_Height(height),
    m_VSync(true)
  {
  }

  void Window::Init(bool hintFloat)
  {

    PSTC_CORE_INFO("Initializing ({1}, {2}) GLFW window: {0}", m_Title, m_Width, m_Height);

    if (!GLFW_Initialized)
    {

      bool success = glfwInit();
      PSTC_CORE_ASSERT(success, "Failed to initialize glfw!");
      PSTC_CORE_TRACE("GLFW initialized!");
      GLFW_Initialized = true;

      glfwSetErrorCallback([](int error, const char* description) {
        PSTC_CORE_ERROR("GLFW Error {0}: {1}", error, description);
        PSTC_DEBUGBREAK();
        });

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
      glfwWindowHint(GLFW_DECORATED, true);
      glfwWindowHint(GLFW_FLOATING, hintFloat);

      m_GLFWWindow = glfwCreateWindow(m_Width, m_Height, &m_Title[0], NULL, NULL);
      PSTC_CORE_ASSERT(m_GLFWWindow, "GLFW window creation failed!");

      m_Device = CreateRef<Device>(*this);

      m_CallbackData.eventLib.Register<WindowCloseEvent>();
      m_CallbackData.eventLib.Register<WindowResizeEvent>();
      m_CallbackData.eventLib.Register<KeyEvent>();
      m_CallbackData.eventLib.Register<MouseClickEvent>();
      m_CallbackData.eventLib.Register<MouseScrollEvent>();
      m_CallbackData.eventLib.Register<MouseMoveEvent>();

      glfwSetWindowUserPointer(m_GLFWWindow, &m_CallbackData);

      glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow* m_GLFWWindow)
        {
          CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(m_GLFWWindow));
          data->eventLib.Publish<WindowCloseEvent>(WindowCloseEvent());
        });

      glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow* m_GLFWWindow, int m_Width, int m_Height)
        {
          CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(m_GLFWWindow));
          data->eventLib.Publish<WindowResizeEvent>(WindowResizeEvent{ m_Width, m_Height });
        });


      glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow* m_GLFWWindow, int key, int scancode, int action, int mods)
        {
          int modFlags = ConvertGLFWModToPistachioMod(mods);

          KeyEvent e
          {
            action == GLFW_PRESS ? Input::ButtonAction::KeyPressed : action == GLFW_RELEASE ? Input::ButtonAction::KeyReleased : Input::ButtonAction::KeyRepeated,
            static_cast<Input::KeyCode>(key),
            modFlags
          };

#ifdef PSTC_VERBOSE_DEBUG
          PSTC_CORE_TRACE("KeyEvent: ({0}, {1}, {2})", e.action, e.key, Input::KeyModFlagToString(e.modFlags));
#endif

          CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(m_GLFWWindow));
          data->eventLib.Publish<KeyEvent>(e);
        }
      );

      glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow* m_GLFWWindow, double x, double y)
        {
          MouseMoveEvent e{ x, y };
#ifdef PSTC_EXTREMELY_VERBOSE_DEBUG
          PSTC_CORE_TRACE("MousePos: ({0}, {1})", e.x, e.y);
#endif
          CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(m_GLFWWindow));
          data->eventLib.Publish<MouseMoveEvent>(e);
        }
      );

      glfwSetMouseButtonCallback(m_GLFWWindow, [](GLFWwindow* m_GLFWWindow, int button, int action, int mods)
        {
          int modFlags = ConvertGLFWModToPistachioMod(mods);

          MouseClickEvent e{
            action == GLFW_PRESS ? Input::ButtonAction::KeyPressed : action == GLFW_RELEASE ? Input::ButtonAction::KeyReleased : Input::ButtonAction::KeyRepeated,
            static_cast<Input::MouseCode>(button),
            modFlags
          };

          glfwGetWindowPos(m_GLFWWindow, &e.x, &e.y);

#ifdef PSTC_VERBOSE_DEBUG
          PSTC_CORE_TRACE("MouseClickEvent: ({0}, {1}, {2}, {3}, {4})", e.action, e.mouseKey, Input::KeyModFlagToString(e.modFlags), e.x, e.y);
#endif

          CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(m_GLFWWindow));
          data->eventLib.Publish<MouseClickEvent>(e);
        }
      );

      glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow* m_GLFWWindow, double xoffset, double yoffset)
        {
          MouseScrollEvent e{ xoffset, yoffset };
          CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(m_GLFWWindow));
          data->ScrollX = xoffset;
          data->ScrollY = yoffset;
#ifdef PSTC_VERBOSE_DEBUG
          PSTC_CORE_TRACE("MouseScrollEvent: ({0}, {1})", e.xoffset, e.yoffset);
#endif
          data->eventLib.Publish<MouseScrollEvent>(e);
        }
      );
    }
  }

  void Window::Shutdown()
  {
    glfwDestroyWindow(m_GLFWWindow);
    glfwTerminate();
  }

  void Window::PollUIEvents()
  {
    m_CallbackData.ScrollX = 0;
    m_CallbackData.ScrollY = 0;
    glfwPollEvents();
  }

  void Pistachio::Window::HideCursor()
  {
    glfwSetInputMode(m_GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
      glfwSetInputMode(m_GLFWWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }

  void Pistachio::Window::ShowCursor()
  {

    glfwSetInputMode(m_GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    if (glfwRawMouseMotionSupported())
      glfwSetInputMode(m_GLFWWindow, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

  }

  bool Window::IsKeyPressed(Input::KeyCode code) const
  {
    GLFWwindow* m_GLFWWindow = glfwGetCurrentContext();
    int state = glfwGetKey(m_GLFWWindow, static_cast<uint16_t>(code));
    return state == (GLFW_PRESS | GLFW_REPEAT);
  }

  bool Window::IsMouseButtonPressed(Input::MouseCode code) const
  {
    GLFWwindow* m_GLFWWindow = glfwGetCurrentContext();
    int state = glfwGetMouseButton(m_GLFWWindow, static_cast<uint16_t>(code));
    return state == (GLFW_PRESS | GLFW_REPEAT);
  }

  glm::dvec2 Window::GetMousePos() const
  {
    GLFWwindow* m_GLFWWindow = glfwGetCurrentContext();
    glm::dvec2 pos;
    glfwGetCursorPos(m_GLFWWindow, &pos.x, &pos.y);
    return pos;
  }

  void Window::SwapBuffers() const
  {
    glfwSwapBuffers(m_GLFWWindow);
  }

  void Window::MakeRenderingContextCurrent() const
  {
    glfwMakeContextCurrent(m_GLFWWindow);
  }

  RenderingAPI::GetRenderAPIProcAddress Window::GetProcAddress() const
  {
    return (RenderingAPI::GetRenderAPIProcAddress)glfwGetProcAddress;
  }

  double Window::GetMouseX() const
  {
    return GetMousePos().x;
  }

  double Window::GetMouseY() const
  {
    return GetMousePos().y;
  }

  double Window::GetScrollX() const
  {
    return m_CallbackData.ScrollX;
  }

  double Window::GetScrollY() const
  {
    return m_CallbackData.ScrollY;
  }

  void Window::SetVSync(bool enabled)
  {
    if (enabled)
    {
      glfwSwapInterval(1);
    }
    else
    {
      glfwSwapInterval(0);
    }

    m_VSync = enabled;
  }

  bool Window::IsVSync() const
  {
    return m_VSync;
  }

}


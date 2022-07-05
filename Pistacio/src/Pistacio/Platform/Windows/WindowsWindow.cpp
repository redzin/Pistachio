#include "pch.h"
#include "WindowsWindow.h"
#include "WindowsInput.cpp"

using namespace std::chrono_literals;

namespace Pistacio
{
  static bool GLFW_Initialized = false;

  void LoadGlad()
  {
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    PSTC_CORE_ASSERT(status, "Failed to to load glad!");
  }

  int ConvertGLFWModToPistacioMod(int mods)
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

  WindowsWindow::WindowsWindow(const WindowProperties& props)
  {
    Init(props);
  }

  void WindowsWindow::Init(const WindowProperties& props)
  {

    PSTC_CORE_INFO("Creating GLFW window {0}: ({1}, {2}) ...", data.Title, data.Width, data.Height);

    data.Title = props.Title;
    data.Width = props.Width;
    data.Height = props.Height;

    if (!GLFW_Initialized)
    {

      bool success = glfwInit();

      if (success)
      {
        GLFW_Initialized = true;
        PSTC_CORE_TRACE("GLFW initialized!");
        glfwSetErrorCallback([](int error, const char* description) {
          PSTC_CORE_ERROR("GLFW Error {0}: {1}", error, description);
          PSTC_DEBUGBREAK();
          });
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);


        glfwWindowHint(GLFW_DECORATED, true);
        glfwWindowHint(GLFW_FLOATING, true);

        glfwWindow = glfwCreateWindow(data.Width, data.Height, &data.Title[0], NULL, NULL);
        PSTC_CORE_ASSERT(glfwWindow, "GLFW window creation failed!");
      }

      glfwMakeContextCurrent(glfwWindow);
      LoadGlad();
      glfwSetWindowUserPointer(glfwWindow, &data);

      glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* glfwWindow)
        {
          Pistacio::WindowCloseEvent e { };
          auto data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));
          data->EventCallback(e);
        });

      glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* glfwWindow, int width, int height)
        {
          Pistacio::WindowResizeEvent e{ width, height };
          auto data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));
          data->EventCallback(e);
        });
      

      glfwSetKeyCallback(glfwWindow, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
        {
          int modFlags = ConvertGLFWModToPistacioMod(mods);
          
          KeyEvent e
          {
            action == GLFW_PRESS ? Input::ButtonAction::KeyPressed : action == GLFW_RELEASE ? Input::ButtonAction::KeyReleased : Input::ButtonAction::KeyRepeated,
            static_cast<Input::KeyCode>(key),
            modFlags
          };

          #ifdef PSTC_VERBOSE_DEBUG
            PSTC_CORE_TRACE("KeyEvent: ({0}, {1}, {2})", e.action, e.key, Input::KeyModFlagToString(e.modFlags));
          #endif

          auto data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));
          data->EventCallback(e);
        }
      );

      glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* glfwWindow, double x, double y)
        {
          MouseMoveEvent e { x,y };
          #ifdef PSTC_VERBOSE_DEBUG
            PSTC_CORE_TRACE("MousePos: ({0}, {1})", e.x, e.y);
          #endif
          auto data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));
          data->EventCallback(e);
        }
      );

      glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* glfwWindow, int button, int action, int mods)
        {
          int modFlags = ConvertGLFWModToPistacioMod(mods);

          MouseClickEvent e {
            action == GLFW_PRESS ? Input::ButtonAction::KeyPressed : action == GLFW_RELEASE ? Input::ButtonAction::KeyReleased : Input::ButtonAction::KeyRepeated,
            static_cast<Input::MouseCode>(button),
            modFlags
          };

          #ifdef PSTC_VERBOSE_DEBUG
            PSTC_CORE_TRACE("MouseClickEvent: ({0}, {1})", e.action, e.mouseKey, Input::KeyModFlagToString(e.modFlags));
          #endif
          auto data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(glfwWindow));
          data->EventCallback(e);
        }
      );
    }
  }

  void WindowsWindow::Shutdown()
  {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
  }

  void WindowsWindow::SetEventCallback(const std::function<void(Event&)> callback)
  {
    data.EventCallback = callback;
  }

  void WindowsWindow::OnUpdate()
  {
    glfwPollEvents();
    glfwSwapBuffers(glfwWindow);
  }

  void WindowsWindow::SetVSync(bool enabled)
  {
    if (enabled)
    {
      glfwSwapInterval(1);
    }
    else
    {
      glfwSwapInterval(0);
    }

    data.VSync = enabled;
  }
  
  bool WindowsWindow::IsVSync() const
  {
    return data.VSync;
  }

}


#include "pch.h"
#include "WindowsWindow.h"
#include "WindowsInput.cpp"

#include "PistacioPlatform/OpenGL/Swapchain_OpenGL.h"

using namespace std::chrono_literals;

namespace Pistacio
{
  static bool GLFW_Initialized = false;

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

  WindowsWindow::WindowsWindow(std::string windowName, uint32_t width, uint32_t height, bool hintFloat)
  {
    Init(windowName, width, height, hintFloat);
  }

  void WindowsWindow::Init(std::string windowName, uint32_t width, uint32_t height, bool hintFloat)
  {

    PSTC_CORE_INFO("Creating ({1}, {2}) GLFW window: {0}", windowName, width, height);

    title = windowName;
    this->width = width;
    this->height = height;

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

      glfwWindow = glfwCreateWindow(width, height, &title[0], NULL, NULL);
      PSTC_CORE_ASSERT(glfwWindow, "GLFW window creation failed!");
      
      swapchain = new Swapchain_OpenGL(glfwWindow);
      swapchain->Init();

      Application::Get()->GetEventLibrary().Register<WindowCloseEvent>();
      Application::Get()->GetEventLibrary().Register<WindowResizeEvent>();
      Application::Get()->GetEventLibrary().Register<KeyEvent>();
      Application::Get()->GetEventLibrary().Register<MouseClickEvent>();
      Application::Get()->GetEventLibrary().Register<MouseMoveEvent>();

      glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* glfwWindow)
        {
          std::shared_ptr<WindowCloseEvent> e  = std::shared_ptr<WindowCloseEvent>(new WindowCloseEvent());
          Application::Get()->GetEventLibrary().Publish<WindowCloseEvent>(e);
        });

      glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* glfwWindow, int width, int height)
        {
          std::shared_ptr<WindowResizeEvent> e = std::shared_ptr<WindowResizeEvent>(new WindowResizeEvent{ width, height });
          Application::Get()->GetEventLibrary().Publish<WindowResizeEvent>(e);
        });
      

      glfwSetKeyCallback(glfwWindow, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
        {
          int modFlags = ConvertGLFWModToPistacioMod(mods);
          
          std::shared_ptr<KeyEvent> e = std::shared_ptr<KeyEvent>(new KeyEvent
            {
              action == GLFW_PRESS ? Input::ButtonAction::KeyPressed : action == GLFW_RELEASE ? Input::ButtonAction::KeyReleased : Input::ButtonAction::KeyRepeated,
              static_cast<Input::KeyCode>(key),
              modFlags
            }
          );

          #ifdef PSTC_VERBOSE_DEBUG
            PSTC_CORE_TRACE("KeyEvent: ({0}, {1}, {2})", e->action, e->key, Input::KeyModFlagToString(e->modFlags));
          #endif

          Application::Get()->GetEventLibrary().Publish<KeyEvent>(e);
        }
      );

      glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* glfwWindow, double x, double y)
        {
          std::shared_ptr<MouseMoveEvent> e = std::shared_ptr<MouseMoveEvent>(new MouseMoveEvent{ x, y });
          #ifdef PSTC_VERBOSE_DEBUG
            PSTC_CORE_TRACE("MousePos: ({0}, {1})", e->x, e->y);
          #endif
          Application::Get()->GetEventLibrary().Publish<MouseMoveEvent>(e);
        }
      );

      glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* glfwWindow, int button, int action, int mods)
        {
          int modFlags = ConvertGLFWModToPistacioMod(mods);

          std::shared_ptr<MouseClickEvent> e = std::shared_ptr<MouseClickEvent>(new MouseClickEvent {
            action == GLFW_PRESS ? Input::ButtonAction::KeyPressed : action == GLFW_RELEASE ? Input::ButtonAction::KeyReleased : Input::ButtonAction::KeyRepeated,
            static_cast<Input::MouseCode>(button),
            modFlags
          });

          #ifdef PSTC_VERBOSE_DEBUG
            PSTC_CORE_TRACE("MouseClickEvent: ({0}, {1})", e->action, e->mouseKey, Input::KeyModFlagToString(e->modFlags));
          #endif

          Application::Get()->GetEventLibrary().Publish<MouseClickEvent>(e);
        }
      );
    }
  }

  void WindowsWindow::Shutdown()
  {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
  }

  void WindowsWindow::Present()
  {
    glfwPollEvents();
    swapchain->SwapBuffers();
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

    vSync = enabled;
  }
  
  bool WindowsWindow::IsVSync() const
  {
    return vSync;
  }

}


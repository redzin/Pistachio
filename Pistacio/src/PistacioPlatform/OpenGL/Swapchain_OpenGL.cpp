#include "pch.h"
#include "Swapchain_OpenGL.h"

Pistacio::Swapchain_OpenGL::Swapchain_OpenGL(GLFWwindow* glfwWindow) : glfwWindow(glfwWindow)
{
  PSTC_CORE_ASSERT(glfwWindow, "GLFWwindow handle is null!");
}

void Pistacio::Swapchain_OpenGL::Init()
{
  glfwMakeContextCurrent(glfwWindow);
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  PSTC_CORE_ASSERT(status, "Failed to to load glad!");

  PSTC_CORE_INFO("OpenGL Info:");
  PSTC_CORE_TRACE("  OpenGL Renderer: {0}", glGetString(GL_RENDERER));
  PSTC_CORE_TRACE("  OpenGL Vendor: {0}", glGetString(GL_VENDOR));
  PSTC_CORE_TRACE("  OpenGL Version: {0}", glGetString(GL_VERSION));
#ifdef PSTC_DEBUG
  const char* ext = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
  if (ext != nullptr)
    PSTC_CORE_TRACE("  OpenGL Extensions: {0}", ext);
#endif
}

void Pistacio::Swapchain_OpenGL::SwapBuffers()
{
  glfwSwapBuffers(glfwWindow);
}

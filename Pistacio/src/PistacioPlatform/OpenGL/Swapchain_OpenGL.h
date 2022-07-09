#pragma once

#include "pch.h"
#include "Pistacio/Rendering/Swapchain.h"

namespace Pistacio
{
  class Swapchain_OpenGL : public Swapchain
  {
  public:
    Swapchain_OpenGL(GLFWwindow* glfwWindow);

    void Init() override;
    void SwapBuffers() override;

  private:
    GLFWwindow* glfwWindow;
  };
}



#include "pch.h"
#include "Pistacio/Core/Application.h"

#include "glm/vec2.hpp"

namespace Pistacio::Input
{

  bool IsKeyPressed(KeyCode code)
  {
    GLFWwindow* glfwWindow = glfwGetCurrentContext();
    int state = glfwGetKey(glfwWindow, static_cast<uint16_t>(code));
    return state == (GLFW_PRESS | GLFW_REPEAT);
  }

  bool IsMouseButtonPressed(MouseCode code)
  {
    GLFWwindow* glfwWindow = glfwGetCurrentContext();
    int state = glfwGetMouseButton(glfwWindow, static_cast<uint16_t>(code));
    return state == (GLFW_PRESS | GLFW_REPEAT);
  }

  glm::dvec2 GetMousePos()
  {
    GLFWwindow* glfwWindow = glfwGetCurrentContext();
    glm::dvec2 pos;
    glfwGetCursorPos(glfwWindow, &pos.x, &pos.y);
  }

  double GetMouseX()
  {
    return GetMousePos().x;
  }

  double GetMouseY()
  {
    return GetMousePos().y;
  }

}

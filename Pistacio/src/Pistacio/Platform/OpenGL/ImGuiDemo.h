#pragma once
#include "pch.h"

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Pistacio
{
  class ImGuiDemo
  {
  public:
    ImGuiDemo() = default;
    ~ImGuiDemo() = default;
    void Init();
    void Render();
    void Cleanup();
  
  private:
  
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  };

}



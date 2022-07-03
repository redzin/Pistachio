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
  class PSTC_API ImGuiRenderer
  {
  public:
    ImGuiRenderer() = default;
    ~ImGuiRenderer() = default;
    void Init();
    void BeginRender();
    void EndRender();
    void Cleanup();

  };

}



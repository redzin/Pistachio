#pragma once

#include "Pistacio/Rendering/ImGui/ImGuiRenderer.h"

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Pistacio
{

  class ImGuiRenderer_GLFW_OpenGL : public ImGuiRenderer
  {
  public:
    ImGuiRenderer_GLFW_OpenGL();
    virtual ~ImGuiRenderer_GLFW_OpenGL();
    virtual void BeginRender() override;
    virtual void EndRender() override;
  };

}

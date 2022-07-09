#include "pch.h"
#include "ImGuiRenderer.h"

#include "PistacioPlatform/OpenGL/ImGuiRenderer_GLFW_OpenGL.h"

namespace Pistacio
{
  ImGuiRenderer* ImGuiRenderer::Create()
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new ImGuiRenderer_GLFW_OpenGL();
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }
  void ImGuiRenderer::HelpMarker(const char* desc)
  {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
      ImGui::BeginTooltip();
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(desc);
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }
  }
}
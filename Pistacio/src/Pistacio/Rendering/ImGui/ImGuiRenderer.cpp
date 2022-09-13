#include "pch.h"
#include "ImGuiRenderer.h"

#include "PistacioPlatform/OpenGL/ImGuiRenderer_GLFW_OpenGL.h"

namespace Pistacio
{
  Scope<ImGuiRenderer> ImGuiRenderer::Create()
  {
    switch (RenderingAPI::GetAPI())
    {
    case RenderingAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RenderingAPI::API::OpenGL:
      return Scope<ImGuiRenderer>(new ImGuiRenderer_GLFW_OpenGL());
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
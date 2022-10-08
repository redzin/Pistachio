#include "PerformanceTracker.h"

void Pistachio::PerformanceTracker::Render(Window& window, glm::vec4& clearColor_out)
{
  ImGui::Begin(m_Name.c_str());

  ImGui::Checkbox("VSync", &m_VSync);

  if (m_VSync != window.IsVSync())
    window.SetVSync(m_VSync);

  ImGui::ColorEdit3("Clear color", (float*)&m_ImguiClearColor);
  clearColor_out.x = m_ImguiClearColor.x;
  clearColor_out.y = m_ImguiClearColor.y;
  clearColor_out.z = m_ImguiClearColor.z;
  clearColor_out.w = m_ImguiClearColor.w;

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();
}

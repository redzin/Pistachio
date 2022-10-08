#pragma once
#include <Pistachio.h>

namespace Pistachio
{


  class PerformanceTracker
  {
  public:
    void Render(Window& window, glm::vec4& clearColor_out);
    std::string GetName() { return m_Name; }
  private:
    bool m_VSync = false;
    ImVec4 m_ImguiClearColor{ 0.7f, 0.85f, 0.92f, 1.00f };
    std::string m_Name = "Performance";
  };

}
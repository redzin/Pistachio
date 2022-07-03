#pragma once
#include "pch.h"
#include "imgui.h"

namespace Pistacio
{
  class ImGuiCounterLayer : public Layer
  {
  public:
    ImGuiCounterLayer(bool useDemo = false);
    ~ImGuiCounterLayer() = default;
    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Event& e) override;
    void OnUpdate() override;
    void OnGuiRender() override;

  private:

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool keybindHasBeenReleased = true;
    int counter = 0;
    int keybind = VK_F1;
    
  };
}



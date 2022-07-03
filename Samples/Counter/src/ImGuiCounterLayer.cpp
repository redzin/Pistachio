#include "pch.h"
#include "ImGuiCounterLayer.h"

namespace Pistacio
{
  using namespace std::chrono_literals;

  ImGuiCounterLayer::ImGuiCounterLayer(bool useDemo) : Layer("ImGuiCounterLayer")
  {
  }

  void ImGuiCounterLayer::OnAttach()
  {
    PSTC_CORE_INFO("ImGuiLayer attached");
  }

  void ImGuiCounterLayer::OnDetach()
  {
    PSTC_CORE_INFO("ImGuiLayer detached");
  }

  void ImGuiCounterLayer::OnEvent(Event& e)
  {

  }
  void ImGuiCounterLayer::OnUpdate()
  {
    bool keybindPressed = (GetAsyncKeyState(keybind)) != 0;
    if (!keybindPressed)
    {
      keybindHasBeenReleased = true;
    }

    if (keybindPressed && keybindHasBeenReleased)
    {
      keybindHasBeenReleased = false;
      counter++;
    }
  }

  void ImGuiCounterLayer::OnGuiRender()
  {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    bool* noCloseButton = NULL;
    ImGui::Begin("Counter", noCloseButton, flags);
    if (ImGui::Button("Add"))
      counter++;
    ImGui::SameLine();
    if (ImGui::Button("Reset"))
      counter = 0;
    ImGui::SameLine();
    ImGui::Text("Counter = %d", counter);
    ImGui::End();
  }

}

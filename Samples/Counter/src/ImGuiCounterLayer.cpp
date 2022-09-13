#include "pch.h"
#include "Pistacio/Core/Application.h"
#include "ImGuiCounterLayer.h"

#include <Windows.h>
#include <iostream>
#include <conio.h>

namespace Pistacio
{
  using namespace std::chrono_literals;

  ImGuiCounterLayer::ImGuiCounterLayer(bool useDemo)
  {
  }

  const char* ImGuiCounterLayer::GetName() const { return "ImGuiCounterLayer"; }

  void ImGuiCounterLayer::OnAttach()
  {
    Application::Get()->GetEventLibrary().Subscribe<AppUpdateEvent>([this](AppUpdateEvent e)
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
        return true;
      }
    );

    PSTC_INFO("ImGuiLayer attached");
  }

  void ImGuiCounterLayer::OnDetach()
  {
    PSTC_INFO("ImGuiLayer detached");
  }

  void ImGuiCounterLayer::OnGuiRender()
  {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    bool* noCloseButton = NULL;

    ImVec2 buttonSize{ 26, 26 };

    ImGui::Begin("Counter", noCloseButton, flags);
    
    ImGui::PushItemWidth(80);
    ImGui::InputScalar("##label", ImGuiDataType_::ImGuiDataType_U32, &counter);
    ImGui::PopItemWidth();
    ImGui::SameLine();


    if (ImGui::Button("+", buttonSize))
      if (counter != UINT_MAX)
        counter++;

    ImGui::SameLine();
    if (ImGui::Button("-", buttonSize))
      if (counter != 0)
        counter--;
   
    ImGui::End();
  }

}

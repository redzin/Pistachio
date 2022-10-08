#include "RenderViewport.h"

namespace Pistachio
{

  bool Viewport::OnViewportMouseClickEvent(ViewportMouseClickEvent& e, Window& window, CameraController_Orbit& cameraControllerOrbit)
  {
    if (e.action == Input::ButtonAction::KeyPressed && e.mouseKey == Input::MouseCode::ButtonRight)
    {
      // do not use for now
      // cameraControllerPan = CameraController_Pan::Enable(cameraControllerPan);
      window.HideCursor();
    }
    else if (e.action == Input::ButtonAction::KeyPressed && e.mouseKey == Input::MouseCode::ButtonLeft)
    {
      cameraControllerOrbit = CameraController_Orbit::Enable(cameraControllerOrbit, e.x, e.y);
      window.HideCursor();
    }
    else if (e.action == Input::ButtonAction::KeyReleased && e.mouseKey == Input::MouseCode::ButtonRight)
    {
      //cameraControllerPan = CameraController_Pan::Disable(cameraControllerPan);
      window.ShowCursor();
    }
    else if (e.action == Input::ButtonAction::KeyReleased && e.mouseKey == Input::MouseCode::ButtonLeft)
    {
      cameraControllerOrbit = CameraController_Orbit::Disable(cameraControllerOrbit, e.x, e.y);
      window.ShowCursor();
    }

    return true;
  }

  bool Viewport::OnViewportMouseMoveEvent(ViewportMouseMoveEvent& e, CameraController_Orbit& cameraControllerOrbit)
  {
    //cameraControllerPan = CameraController_Pan::OnMouseMoveEvent(cameraControllerPan, e);
    cameraControllerOrbit = CameraController_Orbit::OnMouseMoveEvent(cameraControllerOrbit, e);
    return true;
  }

  bool Viewport::OnViewportScrollEvent(ViewportMouseScrollEvent& e, CameraController_Orbit& cameraControllerOrbit)
  {
    cameraControllerOrbit = CameraController_Orbit::OnMouseScrollEvent(cameraControllerOrbit, e);
    return true;
  }

  void Viewport::Render(Window& window, CameraController_Orbit& cameraControllerOrbit, Ref<Attachment> presentTexture)
  {

    ImGuiWindowFlags viewportWindowFlags = 0;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin(m_Name.c_str(), nullptr, viewportWindowFlags);
    //ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImVec2 viewportPanelSize = ImGui::GetWindowSize();
    m_Width = viewportPanelSize.x;
    m_Height = viewportPanelSize.y;

    if (presentTexture > 0)
      ImGui::Image((void *) presentTexture->RendererID, viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::IsWindowHovered() && ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
      ViewportMouseClickEvent e
      {
        Input::ButtonAction::KeyPressed,
        Input::MouseCode::ButtonLeft,
        0,
        ImGui::GetMousePos().x,
        ImGui::GetMousePos().y
      };
      OnViewportMouseClickEvent(e, window, cameraControllerOrbit);
    }

    ViewportMouseScrollEvent e = { window.GetScrollX(), window.GetScrollY() };
    if (ImGui::IsWindowHovered() && ImGui::IsItemHovered() && std::abs(e.yoffset) > 0.1)
    {
      OnViewportScrollEvent(e, cameraControllerOrbit);
      ImGui::SetItemUsingMouseWheel();
    }

    ImGui::End();
    ImGui::PopStyleVar();

    ViewportMouseMoveEvent posEvent{ ImGui::GetMousePos().x, ImGui::GetMousePos().y };
    if (cameraControllerOrbit.enabled && ImGui::IsMouseDown(0))
    {
      OnViewportMouseMoveEvent(posEvent, cameraControllerOrbit);
    }

    if (cameraControllerOrbit.enabled && !ImGui::IsMouseDown(0))
    {
      ViewportMouseClickEvent e
      {
        Input::ButtonAction::KeyReleased,
        Input::MouseCode::ButtonLeft,
        0,
        posEvent.x,
        posEvent.y
      };
      OnViewportMouseClickEvent(e, window, cameraControllerOrbit);
    }

  }

}

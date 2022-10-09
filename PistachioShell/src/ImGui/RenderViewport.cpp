#include "RenderViewport.h"

namespace Pistachio
{

  bool Viewport::OnViewportMouseClickEvent(ViewportMouseClickEvent& e, Window& window, CameraOrbitController& cameraOrbitController)
  {
    if (e.action == Input::ButtonAction::KeyPressed && e.mouseKey == Input::MouseCode::ButtonLeft)
    {
      cameraOrbitController.Enable(e.x, e.y);
      window.HideCursor();
    }
    if (e.action == Input::ButtonAction::KeyReleased && e.mouseKey == Input::MouseCode::ButtonLeft)
    {
      cameraOrbitController.Disable(e.x, e.y);
      window.ShowCursor();
    }

    return true;
  }

  bool Viewport::OnViewportMouseMoveEvent(ViewportMouseMoveEvent& e, CameraOrbitController& cameraOrbitController)
  {
    cameraOrbitController.OnMouseMoveEvent(e);
    return true;
  }

  bool Viewport::OnViewportScrollEvent(ViewportMouseScrollEvent& e, CameraOrbitController& cameraOrbitController)
  {
    cameraOrbitController.OnMouseScrollEvent(e);
    return true;
  }

  void Viewport::Render(Window& window, CameraOrbitController& cameraControllerOrbit, Ref<Attachment> presentTexture)
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
        window.GetMouseX(),
        window.GetMouseY()
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

    ViewportMouseMoveEvent posEvent{ window.GetMouseX(), window.GetMouseY() };
    if (cameraControllerOrbit.Enabled && ImGui::IsMouseDown(0))
    {
      OnViewportMouseMoveEvent(posEvent, cameraControllerOrbit);
    }

    if (cameraControllerOrbit.Enabled && !ImGui::IsMouseDown(0))
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

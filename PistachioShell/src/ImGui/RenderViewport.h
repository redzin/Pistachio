#pragma once
#include <Pistachio.h>
#include "Pistachio/Scene/Camera/CameraController_Orbit.h"

namespace Pistachio
{

  class Viewport
  {
  public:
    Viewport(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) { }

    void Render(Window& window, CameraController_Orbit& cameraControllerOrbit, Ref<Attachment> presentTexture);
    uint32_t GetWidth() { return m_Width; }
    uint32_t GetHeight() { return m_Height; }

  private:

    struct ViewportMouseClickEvent : MouseClickEvent {};
    struct ViewportMouseMoveEvent : MouseMoveEvent {};
    struct ViewportMouseScrollEvent : MouseScrollEvent {};

    bool OnViewportMouseClickEvent(ViewportMouseClickEvent& e, Window& window, CameraController_Orbit& cameraControllerOrbit);
    bool OnViewportMouseMoveEvent(ViewportMouseMoveEvent& e, CameraController_Orbit& cameraControllerOrbit);
    bool OnViewportScrollEvent(ViewportMouseScrollEvent& e, CameraController_Orbit& cameraControllerOrbit);

    uint32_t m_Width;
    uint32_t m_Height;

    std::string m_Name = "Viewport";
  };

}


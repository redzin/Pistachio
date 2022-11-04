#pragma once
#include <Pistachio.h>
#include "../Scene/Camera/CameraOrbitController.h"

namespace Pistachio
{

  class Viewport
  {
  public:
    Viewport(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) { }

    void Render(Window& window, CameraOrbitController& cameraOrbitController, Ref<Attachment> presentTexture);
    uint32_t GetWidth() { return m_Width; }
    uint32_t GetHeight() { return m_Height; }

  private:

    struct ViewportMouseClickEvent : MouseClickEvent {};
    struct ViewportMouseMoveEvent : MouseMoveEvent {};
    struct ViewportMouseScrollEvent : MouseScrollEvent {};

    bool OnViewportMouseClickEvent(ViewportMouseClickEvent& e, Window& window, CameraOrbitController& cameraOrbitController);
    bool OnViewportMouseMoveEvent(ViewportMouseMoveEvent& e, CameraOrbitController& cameraOrbitController);
    bool OnViewportScrollEvent(ViewportMouseScrollEvent& e, CameraOrbitController& cameraOrbitController);

    uint32_t m_Width;
    uint32_t m_Height;

    std::string m_Name = "Viewport";
  };

}


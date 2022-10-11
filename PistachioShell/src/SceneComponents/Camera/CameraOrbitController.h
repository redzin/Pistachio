#pragma once
#include "pch.h"
#include "Camera.h"

namespace Pistachio
{
  class CameraOrbitController
  {
  public:
    void UpdateCamera(Camera& camera, std::chrono::duration<float> dt);
    void OnMouseMoveEvent(MouseMoveEvent& e);
    void OnMouseScrollEvent(MouseScrollEvent e);
    void Enable(double x, double y);
    void Disable(double x, double y);
    Camera CreatePerspectiveCamera(Device& device, glm::vec3 target, glm::vec3 direction, float fovY, uint32_t m_Width, uint32_t m_Height, float zNear, float zFar);

    const bool& Enabled = m_Enabled;

  private:
    // Orbiting
    float m_MouseSpeed = 0.1f;
    double m_X = 0;
    double m_Y = 0;
    double m_Dx = 0;
    double m_Dy = 0;
    double m_DraggedDx = 0;
    double m_DraggedDy = 0;
    float m_Radius = 5;
    bool m_Enabled = false;
    float m_RotationalSpeedRemainingAfterOneSecond = 0.005;
    Timer m_ZeroVelocityTimer;

    // Zooming
    float m_ZoomSpeed = 10.f;
    float m_Dz = 0;
    float m_ZoomSpeedRemainingAfterOneSecond = 0.01;

  };


}

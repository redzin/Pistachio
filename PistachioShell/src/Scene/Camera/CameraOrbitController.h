#pragma once
#include "pch.h"
#include "Camera.h"

namespace Pistachio
{
  class CameraOrbitController
  {
  public:
    CameraOrbitController() = default;
    CameraOrbitController(const CameraOrbitController& t) = default;
    CameraOrbitController(CameraOrbitController&& t) noexcept
    {
      m_MouseSpeed = std::move(t.m_MouseSpeed);
      m_X = std::move(t.m_X);
      m_Y = std::move(t.m_Y);
      m_Dx = std::move(t.m_Dx);
      m_Dy = std::move(t.m_Dy);
      m_DraggedDx = std::move(t.m_DraggedDx);
      m_DraggedDy = std::move(t.m_DraggedDy);
      m_Radius = std::move(t.m_Radius);
      m_Enabled = std::move(t.m_Enabled);
      m_RotationalSpeedRemainingAfterOneSecond = std::move(t.m_RotationalSpeedRemainingAfterOneSecond);
      m_ZeroVelocityTimer = std::move(t.m_ZeroVelocityTimer);
      m_ZoomSpeed = std::move(t.m_ZoomSpeed);
      m_Dz = std::move(t.m_Dz);
      m_ZoomSpeedRemainingAfterOneSecond = std::move(t.m_ZoomSpeedRemainingAfterOneSecond);
    };

    CameraOrbitController& operator=(CameraOrbitController& t) = default;

    CameraOrbitController& operator=(CameraOrbitController&& t) noexcept
    {
      m_MouseSpeed = std::move(t.m_MouseSpeed);
      m_X = std::move(t.m_X);
      m_Y = std::move(t.m_Y);
      m_Dx = std::move(t.m_Dx);
      m_Dy = std::move(t.m_Dy);
      m_DraggedDx = std::move(t.m_DraggedDx);
      m_DraggedDy = std::move(t.m_DraggedDy);
      m_Radius = std::move(t.m_Radius);
      m_Enabled = std::move(t.m_Enabled);
      m_RotationalSpeedRemainingAfterOneSecond = std::move(t.m_RotationalSpeedRemainingAfterOneSecond);
      m_ZeroVelocityTimer = std::move(t.m_ZeroVelocityTimer);
      m_ZoomSpeed = std::move(t.m_ZoomSpeed);
      m_Dz = std::move(t.m_Dz);
      m_ZoomSpeedRemainingAfterOneSecond = std::move(t.m_ZoomSpeedRemainingAfterOneSecond);
      return *this;
    };

    void UpdateCamera(Camera& camera, std::chrono::duration<float> dt);
    void OnMouseMoveEvent(MouseMoveEvent& e);
    void OnMouseScrollEvent(MouseScrollEvent e);
    void Enable(double x, double y);
    void Disable(double x, double y);
    Camera CreatePerspectiveCamera(Device& device, glm::vec3 target, glm::vec3 direction, float radius, float fovY, uint32_t m_Width, uint32_t m_Height, float zNear, float zFar);

    bool IsEnabled() { return m_Enabled; }

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

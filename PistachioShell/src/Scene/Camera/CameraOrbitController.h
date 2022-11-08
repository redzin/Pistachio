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
      m_OrbitingMouseSpeed = std::move(t.m_OrbitingMouseSpeed);
      m_X = std::move(t.m_X);
      m_Y = std::move(t.m_Y);
      m_OrbitDx = std::move(t.m_OrbitDx);
      m_OrbitDy = std::move(t.m_OrbitDy);
      m_PanDx = std::move(t.m_PanDx);
      m_PanDy = std::move(t.m_PanDy);
      m_OrbitDraggedDx = std::move(t.m_OrbitDraggedDx);
      m_OrbitDraggedDy = std::move(t.m_OrbitDraggedDy);
      m_PanDraggedDx = std::move(t.m_PanDraggedDx);
      m_PanDraggedDy = std::move(t.m_PanDraggedDy);
      m_Radius = std::move(t.m_Radius);
      m_OrbitingEnabled = std::move(t.m_OrbitingEnabled);
      m_RotationalSpeedRemainingAfterOneSecond = std::move(t.m_RotationalSpeedRemainingAfterOneSecond);
      m_OrbitZeroVelocityTimer = std::move(t.m_OrbitZeroVelocityTimer);
      m_PanZeroVelocityTimer = std::move(t.m_OrbitZeroVelocityTimer);
      m_ZoomSpeed = std::move(t.m_ZoomSpeed);
      m_Dz = std::move(t.m_Dz);
      m_ZoomSpeedRemainingAfterOneSecond = std::move(t.m_ZoomSpeedRemainingAfterOneSecond);
    };
    
    CameraOrbitController& operator=(CameraOrbitController& t) = default;

    CameraOrbitController& operator=(CameraOrbitController&& t) noexcept
    {
      m_OrbitingMouseSpeed = std::move(t.m_OrbitingMouseSpeed);
      m_PanningMouseSpeed = std::move(t.m_PanningMouseSpeed);
      m_X = std::move(t.m_X);
      m_Y = std::move(t.m_Y);
      m_OrbitDx = std::move(t.m_OrbitDx);
      m_OrbitDy = std::move(t.m_OrbitDy);
      m_PanDx = std::move(t.m_PanDx);
      m_PanDy = std::move(t.m_PanDy);
      m_OrbitDraggedDx = std::move(t.m_OrbitDraggedDx);
      m_OrbitDraggedDy = std::move(t.m_OrbitDraggedDy);
      m_PanDraggedDx = std::move(t.m_PanDraggedDx);
      m_PanDraggedDy = std::move(t.m_PanDraggedDy);
      m_Radius = std::move(t.m_Radius);
      m_OrbitingEnabled = std::move(t.m_OrbitingEnabled);
      m_PanningEnabled = std::move(t.m_PanningEnabled);
      m_RotationalSpeedRemainingAfterOneSecond = std::move(t.m_RotationalSpeedRemainingAfterOneSecond);
      m_OrbitZeroVelocityTimer = std::move(t.m_OrbitZeroVelocityTimer);
      m_PanZeroVelocityTimer = std::move(t.m_PanZeroVelocityTimer);
      m_ZoomSpeed = std::move(t.m_ZoomSpeed);
      m_Dz = std::move(t.m_Dz);
      m_ZoomSpeedRemainingAfterOneSecond = std::move(t.m_ZoomSpeedRemainingAfterOneSecond);
      return *this;
    };

    void UpdateCamera(Camera& camera, std::chrono::duration<float> dt);
    void OnMouseMoveEvent(MouseMoveEvent& e);
    void OnMouseScrollEvent(MouseScrollEvent e);
    void EnableOrbiting(double x, double y);
    void DisableOrbiting(double x, double y);
    void EnablePanning(double x, double y);
    void DisablePanning(double x, double y);
    Camera CreatePerspectiveCamera(Device& device, glm::vec3 target, glm::vec3 direction, float radius, float fovY, uint32_t m_Width, uint32_t m_Height, float zNear, float zFar);

    bool IsOrbitingEnabled() { return m_OrbitingEnabled; }
    bool IsPanningEnabled() { return m_PanningEnabled; }

  private:
    // Position, mouse movement and drag related
    double m_X = 0;
    double m_Y = 0;
    float m_Radius = 5;

    // Orbiting
    void UpdateCameraOrbit(Camera& camera, std::chrono::duration<float> dt);
    float m_OrbitingMouseSpeed = 0.1f;
    float m_RotationalSpeedRemainingAfterOneSecond = 0.005;
    double m_OrbitDraggedDx = 0;
    double m_OrbitDraggedDy = 0;
    double m_OrbitDx = 0;
    double m_OrbitDy = 0;
    bool m_OrbitingEnabled = false;
    Timer m_OrbitZeroVelocityTimer;

    //Panning
    void UpdateCameraPan(Camera& camera, std::chrono::duration<float> dt);
    float m_PanningMouseSpeed = 0.0005f;
    bool m_PanningEnabled = false;
    double m_PanDraggedDx = 0;
    double m_PanDraggedDy = 0;
    double m_PanDx = 0;
    double m_PanDy = 0;
    Timer m_PanZeroVelocityTimer;

    // Zooming
    float m_ZoomSpeed = 10.f;
    float m_Dz = 0;
    float m_ZoomSpeedRemainingAfterOneSecond = 0.01;

  };


}

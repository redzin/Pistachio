#pragma once
#include "pch.h"
#include "Pistachio/Core/Application.h"
#include "Pistachio/Scene/Camera/PerspectiveCamera.h"

namespace Pistachio
{
  struct CameraController_Orbit
  {
    // Orbiting
    float mouseSpeed = 0.1f;
    double lastX = 0;
    double lastY = 0;
    double dx = 0;
    double dy = 0;
    double dx_prev = 0;
    double dy_prev = 0;
    float radius = 5;
    bool enabled = false;
    float rotationalSpeedRemainingAfterOneSecond = 0.005;

    // Zooming
    float zoomSpeed = 10.f;
    float dz = 0;
    float zoomSpeedRemainingAfterOneSecond = 0.01;

    static void UpdateCamera(Camera& camera, std::chrono::duration<float> dt, CameraController_Orbit& state);
    static CameraController_Orbit OnMouseMoveEvent(CameraController_Orbit cameraStateChange, MouseMoveEvent& e);
    static CameraController_Orbit OnMouseScrollEvent(CameraController_Orbit state, MouseScrollEvent e);
    static CameraController_Orbit Enable(CameraController_Orbit state, double x, double y);
    static CameraController_Orbit Disable(CameraController_Orbit state, double x, double y);
    static Camera CreateCamera(CameraController_Orbit state, glm::vec3 target, glm::vec3 direction, float fovY, uint32_t m_Width, uint32_t m_Height, float zNear, float zFar);

  };


}

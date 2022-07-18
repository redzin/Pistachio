#pragma once
#include "pch.h"
#include "Pistacio/Core/Application.h"
#include "Pistacio/Rendering/Camera/PerspectiveCamera.h"

namespace Pistacio
{
  struct CameraController_Orbit
  {
    // Orbiting
    float mouseSpeed = 0.1f;
    double lastX = 0;
    double lastY = 0;
    double dx = 0;
    double dy = 0;
    float radius = 5;
    bool enabled = false;
    float rotationalSpeedRemainingAfterOneSecond = 0.005;

    // Zooming
    float zoomSpeed = 10.f;
    float dz = 0;
    float zoomSpeedRemainingAfterOneSecond = 0.01;

    static std::tuple<Camera, CameraController_Orbit> OnUpdate(Camera camera, std::chrono::duration<float> dt, CameraController_Orbit state);
    static CameraController_Orbit OnMouseMoveEvent(CameraController_Orbit cameraStateChange, MouseMoveEvent& e);
    static CameraController_Orbit OnMouseScrollEvent(CameraController_Orbit state, MouseScrollEvent e);
    static CameraController_Orbit Enable(CameraController_Orbit state);
    static CameraController_Orbit Disable(CameraController_Orbit state);
    static Camera CreateCamera(CameraController_Orbit state, glm::vec3 target, glm::vec3 direction, float fovY, uint32_t width, uint32_t height, float zNear, float zFar);

  };


}

#pragma once
#include "pch.h"
#include "Pistacio/Core/Application.h"
#include "Pistacio/Rendering/Camera/PerspectiveCamera.h"

namespace Pistacio
{
  struct CameraController_FPS
  {
    float mouseSpeed = 0.1f;
    float forwardSpeed = 2.5f;
    float backwardsSpeed = 1.875f;
    double lastX = 0;
    double lastY = 0;
    double yaw = 0;
    double pitch = 0;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveForward = false;
    bool moveBackward = false;
    bool moveUp = false;
    bool moveDown = false;

    static Camera OnUpdate(Camera camera, std::chrono::microseconds dt, CameraController_FPS state);
    static CameraController_FPS OnKeyEvent(CameraController_FPS cameraStateChange, KeyEvent& e);
    static CameraController_FPS OnMouseMoveEvent(CameraController_FPS cameraStateChange, MouseMoveEvent& e);
    static CameraController_FPS StopMovement(CameraController_FPS cameraStateChange);

  };

}

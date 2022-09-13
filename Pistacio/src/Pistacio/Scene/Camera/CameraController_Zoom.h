#pragma once
#include "pch.h"
#include "Pistacio/Core/Application.h"
#include "Pistacio/Scene/Camera/PerspectiveCamera.h"

namespace Pistacio
{
  struct CameraController_Zoom
  {
    float zoomSpeed = 10.f;
    float dz = 0;
    float rotationalSpeedRemainingAfterOneSecond = 0.01;

    static std::tuple<Camera, CameraController_Zoom> UpdateCamera(Camera camera, std::chrono::duration<float> dt, CameraController_Zoom state);
    static CameraController_Zoom OnMouseScrollEvent(CameraController_Zoom state, MouseScrollEvent e);
    
  };

}

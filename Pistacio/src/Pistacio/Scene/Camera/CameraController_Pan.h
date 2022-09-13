#pragma once
#include "pch.h"
#include "Pistacio/Core/Application.h"
#include "Pistacio/Scene/Camera/PerspectiveCamera.h"

namespace Pistacio
{
  enum class CameraController_PanMode
  {
    PanningMovesCamera = 1, PanningMovesWorld = -1
  };

  struct CameraController_Pan
  {
    float mouseSpeed = 1.0f;
    float lastX = 0;
    float lastY = 0;
    float dx = 0;
    float dy = 0;
    float rotationalSpeedRemainingAfterOneSecond = 1;
    CameraController_PanMode mode = CameraController_PanMode::PanningMovesWorld;
    bool enabled = false;

    static Camera UpdateCamera(Camera camera, std::chrono::duration<float> dt, CameraController_Pan state);
    static CameraController_Pan UpdateState(std::chrono::duration<float> dt, CameraController_Pan state);
    static CameraController_Pan OnMouseMoveEvent(CameraController_Pan cameraStateChange, MouseMoveEvent& e, uint32_t windowWidth, uint32_t windowHeight);
    static CameraController_Pan Enable(CameraController_Pan state, Scope<Window>& window);
    static CameraController_Pan Disable(CameraController_Pan state);

  };

}

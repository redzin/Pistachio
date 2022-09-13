#include "pch.h"
#include "CameraController_Zoom.h"

namespace Pistacio
{

  std::tuple<Camera, CameraController_Zoom> CameraController_Zoom::UpdateCamera(Camera camera, std::chrono::duration<float> dt, CameraController_Zoom state)
  {
    float temp = state.rotationalSpeedRemainingAfterOneSecond == 1.0 ? 1.0f : std::log(state.rotationalSpeedRemainingAfterOneSecond);

    glm::vec3 moveVector = -camera.direction * (state.dz) / temp;
    float speed = state.zoomSpeed;
    glm::vec3 newPos = camera.position + speed * dt.count() * moveVector;

    bool valid_pos = !isnan(newPos.x) && !isnan(newPos.y) && !isnan(newPos.z);
    PSTC_ASSERT(valid_pos, "Invalid camera position!");

    Camera newCamera = PerspectiveCamera::NewView(camera, newPos, camera.right, camera.direction);
    

    CameraController_Zoom newState = state;
    // see: https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration
    double dragCoeff = 0.01;
    newState.dz = state.dz * std::pow(dragCoeff, dt.count());
    newState.rotationalSpeedRemainingAfterOneSecond = dragCoeff;

    return std::tuple(newCamera, newState);

  }

  CameraController_Zoom CameraController_Zoom::OnMouseScrollEvent(CameraController_Zoom state, MouseScrollEvent e)
  {
    double z = e.yoffset;
    state.dz += (std::signbit(z) ? -1 : 1) * std::pow(z, 2);
    return state;
  }

}


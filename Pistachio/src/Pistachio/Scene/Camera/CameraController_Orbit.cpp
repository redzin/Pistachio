#include "pch.h"
#include "CameraController_Orbit.h"

namespace Pistachio
{

  void CameraController_Orbit::UpdateCamera(Camera& camera, std::chrono::duration<float> dt, CameraController_Orbit& state)
  {
    if (state.dx == 0 && state.dy == 0 && state.dz == 0)
      return;

    float temp = state.enabled ? 1.0f : -std::log(state.rotationalSpeedRemainingAfterOneSecond);
    float dt_factor = (state.enabled ? 1.0f : dt.count() * 1000);

    double yaw = glm::degrees(glm::atan(camera.direction.z, camera.direction.x));
    double pitch = glm::degrees(glm::asin(camera.direction.y));

    double dx = state.dx_prev;
    double dy = state.dy_prev;

    yaw += dx * state.mouseSpeed * dt_factor / temp;
    pitch += dy * state.mouseSpeed * dt_factor / temp;

    if (state.enabled)
    {
      state.dx_prev = 0;
      state.dy_prev = 0;
    }
    else
    {
      // framerate independent drag
      // see: https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration
      double temp2 = std::pow(0.0001, dt.count());
      state.dx_prev = state.dx_prev * temp2;
      state.dy_prev = state.dy_prev * temp2;
    }


    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0)
      pitch = 89.0;
    if (pitch < -89.0)
      pitch = -89.0f;
    
    if (yaw > 359.0)
      yaw -= 359.0;
    if (yaw < -359.0)
      yaw += 359.0;

    //Direction (mouse movement) update
    float yawRad = glm::radians(yaw);
    float pitchRad = glm::radians(pitch);

    glm::vec3 newDirection = glm::normalize(glm::vec3(
      cos(yawRad) * cos(pitchRad),
      sin(pitchRad),
      sin(yawRad) * cos(pitchRad)
    ));

    glm::vec3 newRight = glm::normalize(glm::vec3(
      cos(yawRad + glm::half_pi<double>()),
      0,
      sin(yawRad + glm::half_pi<double>())
    ));


    glm::vec3 target = camera.position + camera.direction * state.radius;


    float temp3 = state.rotationalSpeedRemainingAfterOneSecond == 1.0 ? 1.0f : std::log(state.rotationalSpeedRemainingAfterOneSecond);
    state.radius += state.zoomSpeed * dt.count() * state.dz / temp3;
    if (state.radius < 0) state.radius = 0;

    glm::vec3 newPosition = target - newDirection * state.radius;

    //CameraController_Orbit newState = state;
    // see: https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration
    state.dz = state.dz * std::pow(state.rotationalSpeedRemainingAfterOneSecond, dt.count());


    //Validate
    bool valid_pos = !isnan(newPosition.x) && !isnan(newPosition.y) && !isnan(newPosition.z);
    PSTC_ASSERT(valid_pos, "Invalid camera position!");

    bool valid_dir = !isnan(newDirection.x) && !isnan(newDirection.y) && !isnan(newDirection.z);
    PSTC_ASSERT(valid_dir, "Invalid camera position!");

    camera = PerspectiveCamera::NewView(camera, newPosition, newRight, newDirection);
    //return std::tuple(camera, state);

  }

  CameraController_Orbit CameraController_Orbit::OnMouseMoveEvent(CameraController_Orbit state, MouseMoveEvent& e)
  {

    if (!state.enabled)
      return state;

    double x = e.x;
    double y = e.y;
    
    double dx = x - state.lastX;
    double dy = state.lastY - y;

    if (std::abs(dx) < 1 && std::abs(dy) < 1)
      return state;

    state.dx_prev = state.dx;
    state.dy_prev = state.dy;
    state.dx = dx;
    state.dy = dy;
    state.lastX = x;
    state.lastY = y;

    return state;
  }

  CameraController_Orbit CameraController_Orbit::OnMouseScrollEvent(CameraController_Orbit state, MouseScrollEvent e)
  {
    double z = e.yoffset;
    state.dz += (std::signbit(z) ? -1 : 1) * std::pow(z, 2);
    return state;
  }

  CameraController_Orbit CameraController_Orbit::Enable(CameraController_Orbit state, double x, double y)
  {
    CameraController_Orbit newState;
    newState.lastX = x;
    newState.lastY = y;
    newState.radius = state.radius;
    newState.enabled = true;
    return newState;
  }

  CameraController_Orbit CameraController_Orbit::Disable(CameraController_Orbit state, double x, double y)
  {

    //double dx = x - state.lastX;
    //double dy = state.lastY - y;
    //state.lastX = x;
    //state.lastY = y;

    state.dx_prev = state.dx;
    state.dy_prev = state.dy;

    state.enabled = false;
    return state;
  }

  Camera CameraController_Orbit::CreateCamera(CameraController_Orbit state, glm::vec3 target, glm::vec3 direction, float fovY, uint32_t m_Width, uint32_t m_Height, float zNear, float zFar)
  {
    glm::vec3 position = target - glm::normalize(direction) * state.radius;
    return PerspectiveCamera::Create(position, target, fovY, m_Width, m_Height, zNear, zFar);
  }

  

}

#include "pch.h"
#include "CameraOrbitController.h"

namespace Pistachio
{
  void CameraOrbitController::UpdateCameraOrbit(Camera& camera, std::chrono::duration<float> dt)
  {
    if (std::abs(m_OrbitDx) > 0 || std::abs(m_OrbitDy) > 0)
      m_OrbitZeroVelocityTimer = Timer();

    if (m_OrbitZeroVelocityTimer.getElapsed<std::chrono::milliseconds>() > 25)
    {
      m_OrbitDraggedDx = 0;
      m_OrbitDraggedDy = 0;
    }
    else
    {
      // framerate independent drag
      // see: https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration
      double temp2 = std::pow(0.0001, dt.count());
      m_OrbitDraggedDx = m_OrbitDraggedDx * temp2;
      m_OrbitDraggedDy = m_OrbitDraggedDy * temp2;
    }

    if (m_OrbitingEnabled && m_OrbitDx == 0 && m_OrbitDy == 0 && m_Dz == 0 && m_OrbitDraggedDx == 0 && m_OrbitDraggedDy == 0)
      return;

    float temp = m_OrbitingEnabled ? 1.0f : -std::log(m_RotationalSpeedRemainingAfterOneSecond);
    float dt_factor = (m_OrbitingEnabled ? 1.0f : dt.count() * 1000);

    glm::vec3 currentDir = camera.GetDirection();
    glm::vec3 currentPos = camera.GetPosition();

    double yaw = glm::degrees(glm::atan(currentDir.z, currentDir.x));
    double pitch = glm::degrees(glm::asin(currentDir.y));

    yaw += m_OrbitDx * m_OrbitingMouseSpeed * dt_factor / temp;
    pitch += m_OrbitDy * m_OrbitingMouseSpeed * dt_factor / temp;

    if (m_OrbitingEnabled)
    {
      m_OrbitDx = 0;
      m_OrbitDy = 0;

    }
    else
    {
      m_OrbitDx = m_OrbitDraggedDx;
      m_OrbitDy = m_OrbitDraggedDy;
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

    glm::vec3 target = currentPos + currentDir * m_Radius;

    float temp3 = m_RotationalSpeedRemainingAfterOneSecond == 1.0 ? 1.0f : std::log(m_RotationalSpeedRemainingAfterOneSecond);
    m_Radius += m_ZoomSpeed * dt.count() * m_Dz / temp3;
    if (m_Radius < 0)
      m_Radius = 0;

    glm::vec3 newPosition = target - newDirection * m_Radius;

    // see: https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration
    m_Dz = m_Dz * std::pow(m_RotationalSpeedRemainingAfterOneSecond, dt.count());

    bool valid_pos = !isnan(newPosition.x) && !isnan(newPosition.y) && !isnan(newPosition.z);
    PSTC_ASSERT(valid_pos, "Invalid camera position!");

    bool valid_dir = !isnan(newDirection.x) && !isnan(newDirection.y) && !isnan(newDirection.z);
    PSTC_ASSERT(valid_dir, "Invalid camera position!");

    camera.UpdateView(newPosition, newRight, newDirection);

  }

  void CameraOrbitController::UpdateCameraPan(Camera& camera, std::chrono::duration<float> dt)
  {

    if (std::abs(m_PanDx) > 0 || std::abs(m_PanDx) > 0)
      m_PanZeroVelocityTimer = Timer();

    if (m_PanZeroVelocityTimer.getElapsed<std::chrono::milliseconds>() > 25)
    {
      m_PanDraggedDx = 0;
      m_PanDraggedDy = 0;
    }
    else
    {
      // framerate independent drag
      // see: https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration
      double temp = std::pow(0.0001, dt.count());
      m_PanDraggedDx = m_PanDraggedDx * temp;
      m_PanDraggedDy = m_PanDraggedDy * temp;
    }

    float dx = 0;
    float dy = 0;

    if (m_PanningEnabled)
    {
      dx = m_PanDx;
      dy = m_PanDy;
      m_PanDx = 0;
      m_PanDy = 0;
    }
    else
    {
      m_PanDx = m_PanDraggedDx;
      m_PanDy = m_PanDraggedDy;
      dx = m_PanDx;
      dy = m_PanDy;
    }

    glm::vec3 currentDir = camera.GetDirection();
    glm::vec3 currentPos = camera.GetPosition();
    glm::vec3 currentRight = camera.GetRight();
    glm::vec3 currentUp = camera.GetUp();

    glm::vec3 newPosition = camera.GetPosition() - m_PanningMouseSpeed * (m_Radius + 0.05f) * (dx * currentRight + dy * currentUp);

    bool valid_pos = !isnan(newPosition.x) && !isnan(newPosition.y) && !isnan(newPosition.z);
    PSTC_ASSERT(valid_pos, "Invalid camera position!");

    camera.UpdateView(newPosition, currentRight, currentDir);

    
  }

  void CameraOrbitController::UpdateCamera(Camera& camera, std::chrono::duration<float> dt)
  {
    UpdateCameraOrbit(camera, dt);
    UpdateCameraPan(camera, dt);
  }

  void CameraOrbitController::OnMouseMoveEvent(MouseMoveEvent& e)
  {
    if (!m_OrbitingEnabled && !m_PanningEnabled)
      return;

    double dx = e.x - m_X;
    double dy = m_Y - e.y;

    if (m_OrbitingEnabled)
    {
      m_OrbitDraggedDx = std::abs(dx) > std::abs(m_OrbitDraggedDx) ? dx : m_OrbitDraggedDx;
      m_OrbitDraggedDy = std::abs(dy) > std::abs(m_OrbitDraggedDy) ? dy : m_OrbitDraggedDy;
      m_OrbitDx = dx;
      m_OrbitDy = dy;
    }

    if (m_PanningEnabled)
    {
      m_PanDraggedDx = std::abs(dx) > std::abs(m_PanDraggedDx) ? dx : m_PanDraggedDx;
      m_PanDraggedDy = std::abs(dy) > std::abs(m_PanDraggedDy) ? dy : m_PanDraggedDy;
      m_PanDx = dx;
      m_PanDy = dy;
    }

    if (std::abs(dx) < 1 && std::abs(dy) < 1)
      return;
    
    m_X = e.x;
    m_Y = e.y;
  }

  void CameraOrbitController::OnMouseScrollEvent(MouseScrollEvent e)
  {
    double z = e.yoffset;
    m_Dz += (std::signbit(z) ? -1 : 1) * std::pow(z, 2);
  }

  void CameraOrbitController::EnableOrbiting(double x, double y)
  {
    CameraOrbitController newState;
    m_X = x;
    m_Y = y;
    
    m_OrbitDx = newState.m_OrbitDx;
    m_OrbitDy = newState.m_OrbitDy;
    m_Dz = newState.m_Dz;

    m_OrbitingEnabled = true;
  }

  void CameraOrbitController::DisableOrbiting(double x, double y)
  {
    m_OrbitingEnabled = false;
  }

  void CameraOrbitController::EnablePanning(double x, double y)
  {

    CameraOrbitController newState;
    m_X = x;
    m_Y = y;
    
    m_PanDx = newState.m_PanDx;
    m_PanDy = newState.m_PanDy;
    
    m_PanningEnabled = true;

  }

  void CameraOrbitController::DisablePanning(double x, double y)
  {
    m_PanningEnabled = false;
  }

  Camera CameraOrbitController::CreatePerspectiveCamera(Device& device, glm::vec3 target, glm::vec3 direction, float radius, float fovY, uint32_t width, uint32_t height, float zNear, float zFar)
  {
    m_Radius = radius;
    glm::vec3 position = target - glm::normalize(direction) * m_Radius;
    return Camera::CreatePerspectiveCamera(device, position, target, fovY, width == 0 ? 1920 : width, height == 0 ? 1080 : height, zNear, zFar);
  }

  

}

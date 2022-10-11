#include "pch.h"
#include "CameraOrbitController.h"

namespace Pistachio
{

  void CameraOrbitController::UpdateCamera(Camera& camera, std::chrono::duration<float> dt)
  {

    if (std::abs(m_Dx) > 0 || std::abs(m_Dy) > 0)
      m_ZeroVelocityTimer = Timer();

    if (m_ZeroVelocityTimer.getElapsed<std::chrono::milliseconds>() > 25)
    {
      m_DraggedDx = 0;
      m_DraggedDy = 0;
    }
    else
    {
      // framerate independent drag
      // see: https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration
      double temp2 = std::pow(0.0001, dt.count());
      m_DraggedDx = m_DraggedDx * temp2;
      m_DraggedDy = m_DraggedDy * temp2;
    }

    if (m_Enabled && m_Dx == 0 && m_Dy == 0 && m_Dz == 0 && m_DraggedDx == 0 && m_DraggedDy == 0)
      return;

    float temp = m_Enabled ? 1.0f : -std::log(m_RotationalSpeedRemainingAfterOneSecond);
    float dt_factor = (m_Enabled ? 1.0f : dt.count() * 1000);

    double yaw = glm::degrees(glm::atan(camera.Direction.z, camera.Direction.x));
    double pitch = glm::degrees(glm::asin(camera.Direction.y));

    yaw += m_Dx * m_MouseSpeed * dt_factor / temp;
    pitch += m_Dy * m_MouseSpeed * dt_factor / temp;

    if (m_Enabled)
    {
      m_Dx = 0;
      m_Dy = 0;

    }
    else
    {
      m_Dx = m_DraggedDx;
      m_Dy = m_DraggedDy;
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

    glm::vec3 target = camera.Position + camera.Direction * m_Radius;

    float temp3 = m_RotationalSpeedRemainingAfterOneSecond == 1.0 ? 1.0f : std::log(m_RotationalSpeedRemainingAfterOneSecond);
    m_Radius += m_ZoomSpeed * dt.count() * m_Dz / temp3;
    if (m_Radius < 0)
      m_Radius = 0;
    
    glm::vec3 newPosition = target - newDirection * m_Radius;

    // see: https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration
    m_Dz = m_Dz * std::pow(m_RotationalSpeedRemainingAfterOneSecond, dt.count());

    //Validate
    bool valid_pos = !isnan(newPosition.x) && !isnan(newPosition.y) && !isnan(newPosition.z);
    PSTC_ASSERT(valid_pos, "Invalid camera position!");

    bool valid_dir = !isnan(newDirection.x) && !isnan(newDirection.y) && !isnan(newDirection.z);
    PSTC_ASSERT(valid_dir, "Invalid camera position!");

    camera.UpdateView(newPosition, newRight, newDirection);
    
  }

  void CameraOrbitController::OnMouseMoveEvent(MouseMoveEvent& e)
  {
    if (!m_Enabled)
      return;

    double dx = e.x - m_X;
    double dy = m_Y - e.y;
    m_DraggedDx = std::abs(dx) > std::abs(m_DraggedDx) ? dx : m_DraggedDx;
    m_DraggedDy = std::abs(dy) > std::abs(m_DraggedDy) ? dy : m_DraggedDy;

    if (std::abs(dx) < 1 && std::abs(dy) < 1)
      return;
    

    m_Dx = dx;
    m_Dy = dy;
    m_X = e.x;
    m_Y = e.y;
  }

  void CameraOrbitController::OnMouseScrollEvent(MouseScrollEvent e)
  {
    double z = e.yoffset;
    m_Dz += (std::signbit(z) ? -1 : 1) * std::pow(z, 2);
  }

  void CameraOrbitController::Enable(double x, double y)
  {
    CameraOrbitController newState;
    m_X = x;
    m_Y = y;
    
    m_Dx = newState.m_Dx;
    m_Dy = newState.m_Dy;
    m_Dz = newState.m_Dz;

    m_Enabled = true;
  }

  void CameraOrbitController::Disable(double x, double y)
  {
    m_Enabled = false;
  }

  Camera CameraOrbitController::CreatePerspectiveCamera(Device& device, glm::vec3 target, glm::vec3 direction, float fovY, uint32_t m_Width, uint32_t m_Height, float zNear, float zFar)
  {
    glm::vec3 position = target - glm::normalize(direction) * m_Radius;
    return Camera::CreatePerspectiveCamera(device, position, target, fovY, m_Width, m_Height, zNear, zFar);
  }

  

}

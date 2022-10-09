#include "Camera.h"

namespace Pistachio 
{

  Camera::Camera(
    Device& device,
    glm::mat4 view,
    glm::mat4 projection,
    glm::vec3 position,
    glm::vec3 direction,
    glm::vec3 up,
    glm::vec3 right,
    glm::uvec2 viewportDimensions
  ) :
    m_View(view),
    m_Projection(projection),
    m_Position(position),
    m_Direction(direction),
    m_Up(up),
    m_Right(right),
    m_ViewportDimensions(viewportDimensions)
  {

    BufferDescriptor bufferDesc;
    bufferDesc.Size = 4 * 4 * 4;
    bufferDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;

    m_ViewBuffer = device.CreateBuffer(bufferDesc, &view[0]);
    m_ProjectionBuffer = device.CreateBuffer(bufferDesc, &projection[0]);
    m_ViewProjectionBuffer = device.CreateBuffer(bufferDesc);
    UpdateViewProjectionBuffer();
  }

  Camera::Camera(Camera&& t) :
  View(m_View),
  Projection(m_Projection),
  Position(m_Position),
  Direction(m_Direction),
  Up(m_Up),
  Right(m_Right),
  ViewportDimensions(m_ViewportDimensions),
  ViewBuffer(m_ViewBuffer),
  ProjectionBuffer(m_ProjectionBuffer),
  ViewProjectionBuffer(m_ViewProjectionBuffer),
  m_View(std::move(t.m_View)),
  m_Projection(std::move(t.m_Projection)),
  m_Position(std::move(t.m_Position)),
  m_Direction(std::move(t.m_Direction)),
  m_Up(std::move(t.m_Up)),
  m_Right(std::move(t.m_Right)),
  m_ViewportDimensions(std::move(t.m_ViewportDimensions)),
  m_ViewBuffer(std::move(t.m_ViewBuffer)),
  m_ProjectionBuffer(std::move(t.m_ProjectionBuffer)),
  m_ViewProjectionBuffer(std::move(t.m_ViewProjectionBuffer))
  {
  }

  void Camera::UpdateViewBuffer()
  {
    memcpy(ViewProjectionBuffer->MemoryPtr, &m_View[0], sizeof(glm::mat4));
  }

  void Camera::UpdateProjectionBuffer()
  {
    memcpy(ViewProjectionBuffer->MemoryPtr, &m_Projection[0], sizeof(glm::mat4));
  }

  void Camera::UpdateViewProjectionBuffer()
  {
    glm::mat4 viewProjection = m_Projection * m_View;
    memcpy(ViewProjectionBuffer->MemoryPtr, &viewProjection[0], sizeof(glm::mat4));
  }

  Camera Camera::CreatePerspectiveCamera(Device& device, glm::vec3 position, glm::vec3 target, float fovY, uint32_t width, uint32_t height, float zNear, float zFar)
  {
    glm::vec3 direction = glm::normalize(target - position);
    glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    glm::vec3 right = glm::normalize(glm::cross(direction, up));
    up = glm::normalize(glm::cross(right, direction));

    glm::mat4 viewMatrix = glm::lookAt(position, direction, up);
    glm::mat4 projectionMatrix = glm::perspective(fovY, (float)width / height, zNear, zFar);

    glm::uvec2 viewport{ width, height };

    return Camera(device, viewMatrix, projectionMatrix, position, direction, up, right, viewport);
  }

  void Camera::UpdateView(glm::vec3 position, glm::vec3 right, glm::vec3 direction)
  {
    bool dir_not_zero = glm::dot(direction, direction) != 0;
    PSTC_ASSERT(dir_not_zero, "Cannot set camera direction to zero!");

    m_Position = position;
    m_Right = right;
    m_Direction = direction;
    m_Up = glm::cross(right, direction);
    m_View = glm::lookAt(position, position + direction, Up);
    UpdateViewBuffer();
    UpdateViewProjectionBuffer();
  }

  void Camera::UpdateProjection(float fovY, uint32_t width, uint32_t height, float zNear, float zFar)
  {
    m_Projection = glm::perspective(fovY, (float)width / height, zNear, zFar);
    m_ViewportDimensions = { width, height };
    UpdateProjectionBuffer();
    UpdateViewProjectionBuffer();
  }
}


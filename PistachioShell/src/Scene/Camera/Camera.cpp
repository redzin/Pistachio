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
    bufferDesc.Size = 4 * 4 * 4 * 3 + 4 * 3; // holds three 4x4 matrices with 4-byte entries (e.g. floats). View, Projection, Projection*View,
                                             // and finally holds 3-component vector with the position
    bufferDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;

    m_GPUBuffer = device.CreateBuffer(bufferDesc);

    UpdateViewBuffer();
    UpdateProjectionBuffer();
    UpdateViewProjectionBuffer();
  }

  void Camera::UpdateViewBuffer()
  {
    m_GPUBuffer->Wait();
    memcpy(&((glm::mat4*)m_GPUBuffer->MemoryPtr)[0], &m_View[0], sizeof(glm::mat4));
  }

  void Camera::UpdateProjectionBuffer()
  {
    m_GPUBuffer->Wait();
    memcpy(&((glm::mat4*)m_GPUBuffer->MemoryPtr)[1], &m_Projection[0], sizeof(glm::mat4));
  }

  void Camera::UpdateViewProjectionBuffer()
  {
    glm::mat4 viewProjection = m_Projection * m_View;
    m_GPUBuffer->Wait();
    memcpy(&((glm::mat4*)m_GPUBuffer->MemoryPtr)[2], &viewProjection[0], sizeof(glm::mat4));
  }

  void Camera::UpdatePositionBuffer()
  {
    m_GPUBuffer->Wait();
    memcpy(&((glm::vec4*)m_GPUBuffer->MemoryPtr)[12], &m_Position[0], sizeof(glm::vec4));
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
    m_View = glm::lookAt(position, position + direction, m_Up);
    UpdateViewBuffer();
    UpdateViewProjectionBuffer();
    UpdatePositionBuffer();
  }

  void Camera::UpdateProjection(float fovY, uint32_t width, uint32_t height, float zNear, float zFar)
  {
    m_Projection = glm::perspective(fovY, (float)width / height, zNear, zFar);
    m_ViewportDimensions = { width, height };
    UpdateProjectionBuffer();
    UpdateViewProjectionBuffer();
  }

  glm::mat4 Camera::GetView()
  {
    return ((glm::mat4*)m_GPUBuffer->MemoryPtr)[0];
  }
  glm::mat4 Camera::GetProjection()
  {
    return ((glm::mat4*)m_GPUBuffer->MemoryPtr)[1];
  }
}


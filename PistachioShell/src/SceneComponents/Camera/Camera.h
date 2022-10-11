#pragma once
#include "pch.h"

namespace Pistachio
{
  struct PrimaryCameraTag {};
  struct EditorCameraTag {};

  struct Camera {
    Camera& operator=(Camera&) = default;
    Camera(Camera&&);
    Camera& operator=(Camera&&) noexcept = default;
    ~Camera() = default;

    static Camera CreatePerspectiveCamera(Device& device, glm::vec3 position, glm::vec3 target, float fovY, uint32_t m_Width, uint32_t m_Height, float zNear, float zFar);
    void UpdateView(glm::vec3 position, glm::vec3 right, glm::vec3 direction);
    void UpdateProjection(float fovY, uint32_t m_Width, uint32_t m_Height, float zNear, float zFar);

    glm::mat4 GetView();
    glm::mat4 GetProjection();

    const glm::vec3& Position = m_Position;
    const glm::vec3& Direction = m_Direction;
    const glm::vec3& Up = m_Up;
    const glm::vec3& Right = m_Right;
    const glm::uvec2& ViewportDimensions = m_ViewportDimensions;

    const Ref<Buffer>& GPUBuffer = m_GPUBuffer;

  private:
    Camera(Camera&) = default; // do not support copying

    Camera(
      Device& device,
      glm::mat4 view,
      glm::mat4 projection,
      glm::vec3 position,
      glm::vec3 direction,
      glm::vec3 up,
      glm::vec3 right,
      glm::uvec2 viewportDimensions
    );
  private:
    void UpdateViewBuffer();
    void UpdateProjectionBuffer();
    void UpdateViewProjectionBuffer();

  private:

    glm::mat4 m_View;
    glm::mat4 m_Projection;
    glm::vec3 m_Position;
    glm::vec3 m_Direction;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::uvec2 m_ViewportDimensions;

    Ref<Buffer> m_GPUBuffer; // contains view, projection, and proj*view matrices in that order
  };
}


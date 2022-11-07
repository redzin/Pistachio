#pragma once
#include "pch.h"

namespace Pistachio
{

  struct Camera {

    Camera& operator=(Camera& t) // copy assignment
    {
      m_View = t.m_View;
      m_Projection = t.m_Projection;
      m_Position = t.m_Position;
      m_Direction = t.m_Direction;
      m_Up = t.m_Up;
      m_Right = t.m_Right;
      m_ViewportDimensions = t.m_ViewportDimensions;
      m_GPUBuffer = t.m_GPUBuffer;
      PSTC_INFO("Camera copy assignment!");
      return *this;
    }
    
    Camera(Camera&& t) noexcept : // move constructor
      m_View(std::move(t.m_View)),
      m_Projection(std::move(t.m_Projection)),
      m_Position(std::move(t.m_Position)),
      m_Direction(std::move(t.m_Direction)),
      m_Up(std::move(t.m_Up)),
      m_Right(std::move(t.m_Right)),
      m_ViewportDimensions(std::move(t.m_ViewportDimensions)),
      m_GPUBuffer(t.m_GPUBuffer)
    {
      PSTC_INFO("Camera move constructor!");
    }
    
    Camera& operator=(Camera&& t) noexcept // move assignment
    {
      m_View = std::move(t.m_View);
      m_Projection = std::move(t.m_Projection);
      m_Position = std::move(t.m_Position);
      m_Direction = std::move(t.m_Direction);
      m_Up = std::move(t.m_Up);
      m_Right = std::move(t.m_Right);
      m_ViewportDimensions = std::move(t.m_ViewportDimensions);
      m_GPUBuffer = std::move(t.m_GPUBuffer);
      PSTC_INFO("Camera move assignment!");
      return *this;
    }

    //Camera& operator=(Camera& t) = default;
    //Camera(Camera&& t) noexcept = default;
    //Camera& operator=(Camera&& t) noexcept = default;
    Camera(Camera& t) = default;
    ~Camera() = default;

    static Camera CreatePerspectiveCamera(Device& device, glm::vec3 position, glm::vec3 target, float fovY, uint32_t m_Width, uint32_t m_Height, float zNear, float zFar);
    void UpdateView(glm::vec3 position, glm::vec3 right, glm::vec3 direction);
    void UpdateProjection(float fovY, uint32_t m_Width, uint32_t m_Height, float zNear, float zFar);

    glm::mat4 GetView();
    glm::mat4 GetProjection();
    glm::vec3 GetPosition() { return m_Position; };
    glm::vec3 GetDirection() { return m_Direction; };
    glm::vec3 GetUp() { return m_Up; };
    glm::vec3 GetRight() { return m_Right; };
    glm::uvec2 GetViewportDimensions() { return m_ViewportDimensions; };
    Ref<Buffer> GetBuffer() { return m_GPUBuffer; };

  private:
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
    void UpdatePositionBuffer();

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


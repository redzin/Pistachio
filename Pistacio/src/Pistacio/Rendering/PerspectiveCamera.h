#pragma once

#include "pch.h"

namespace Pistacio
{

  class PerspectiveCamera
  {
  public:
    struct Data {
      glm::mat4 viewMatrix;
      glm::mat4 projectionMatrix;
      glm::vec3 position;
      glm::vec3 direction;
      glm::vec3 up;
    };

    PerspectiveCamera(glm::vec3 position, glm::vec3 target, float fovY, uint32_t width, uint32_t height, float zNear, float zFar);
    ~PerspectiveCamera() = default;

    const glm::mat4 GetProjectionMatrix() const { return data.projectionMatrix; }
    const glm::mat4 GetViewMatrix() const { return data.viewMatrix; }
    const glm::vec3 GetPosition() const { return data.position; }
    const glm::vec3 GetDirection() const { return data.direction; }
    const glm::vec3 GetUp() const { return data.up; }
    const Data* GetDataPtr() const { return &data; };

  private:
    Data data;
  };

}
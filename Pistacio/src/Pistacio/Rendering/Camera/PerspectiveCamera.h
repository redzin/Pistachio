#pragma once

#include "pch.h"
#include "Camera.h"

namespace Pistacio
{

  class PerspectiveCamera
  {
  public:

    static Camera Create(glm::vec3 position, glm::vec3 target, float fovY, uint32_t width, uint32_t height, float zNear, float zFar);
    static Camera NewView(Camera& oldCamera, glm::vec3 position, glm::vec3 right, glm::vec3 direction);
    static Camera NewPerspective(Camera& oldCamera, float fovY, uint32_t width, uint32_t height, float zNear, float zFar);
  };

}
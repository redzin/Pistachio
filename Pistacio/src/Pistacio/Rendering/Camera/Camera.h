#pragma once
#include <glm/glm.hpp>

namespace Pistacio
{

  struct Camera {
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;
  };
}

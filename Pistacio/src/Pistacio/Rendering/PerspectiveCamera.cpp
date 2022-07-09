#include "pch.h"
#include "PerspectiveCamera.h"

namespace Pistacio
{
  PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 target, float fovY, uint32_t width, uint32_t height, float zNear, float zFar)
    : data(Data{glm::mat4(1), glm::mat4(0), position, glm::vec3(), glm::vec3() })
  {
    
    data.direction = target - position;
    data.up = {0.0f, 1.0f, 0.0f};
    glm::vec3 right = glm::cross(data.direction, data.up);
    data.up = glm::normalize(glm::cross(right, data.direction));

    data.viewMatrix = glm::lookAt(position, data.direction, data.up);
    data.projectionMatrix = glm::perspective(fovY, (float)width / height, zNear, zFar);

    //data.projectionMatrix = glm::mat4(1);

  }

}


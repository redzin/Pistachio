#include "pch.h"
#include "PerspectiveCamera.h"

namespace Pistachio
{
    Camera PerspectiveCamera::Create(glm::vec3 position, glm::vec3 target, float fovY, uint32_t width, uint32_t height, float zNear, float zFar)
  {
    glm::vec3 direction = glm::normalize(target - position);
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
    glm::vec3 right = glm::normalize(glm::cross(direction, up));
    up = glm::normalize(glm::cross(right, direction));

    glm::mat4 viewMatrix = glm::lookAt(position, direction, up);
    glm::mat4 projectionMatrix = glm::perspective(fovY, (float)width / height, zNear, zFar);

    glm::uvec2 viewport{ width, height };

    return Camera(viewMatrix, projectionMatrix, position, direction, up, right, viewport);
  }

  Camera PerspectiveCamera::NewView(Camera& oldCamera, glm::vec3 position, glm::vec3 right, glm::vec3 direction)
  {
    bool dir_not_zero = glm::dot(direction, direction) != 0;
    PSTC_ASSERT(dir_not_zero, "Cannot set camera direction to zero!");

    glm::vec3 up = glm::cross(right, direction);
    glm::mat4 viewMat = glm::lookAt(position, position + direction, up);

    Camera cam(viewMat, oldCamera.projectionMatrix, position, direction, up, right, oldCamera.viewport);
    return cam;
  }

  Camera PerspectiveCamera::NewPerspective(Camera& oldCamera, float fovY, uint32_t width, uint32_t height, float zNear, float zFar)
  {
    glm::mat4 projectionMatrix = glm::perspective(fovY, (float)width / height, zNear, zFar);
    glm::uvec2 viewport{ width, height };
    return Camera(oldCamera.viewMatrix, projectionMatrix, oldCamera.position, oldCamera.direction, oldCamera.up, oldCamera.right, viewport);
  }
}


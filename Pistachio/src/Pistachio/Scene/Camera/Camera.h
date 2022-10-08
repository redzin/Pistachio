#pragma once
#include <glm/glm.hpp>

namespace Pistachio
{

  struct Camera {
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;
    glm::uvec2 viewport;

    //Camera() = default;
    
    //Camera(
    //  glm::mat4 viewMatrix,
    //  glm::mat4 projectionMatrix,
    //  glm::vec3 position,
    //  glm::vec3 direction,
    //  glm::vec3 up,
    //  glm::vec3 right,
    //  glm::uvec2 viewport
    //)
    //: viewMatrix(viewMatrix),
    //  projectionMatrix(projectionMatrix),
    //  position(position),
    //  direction(direction),
    //  up(up),
    //  right(right),
    //  viewport(viewport)
    //{
    //}
    //
    //Camera(Camera&& other) noexcept
    //: viewMatrix(std::move(other.viewMatrix)),
    //  projectionMatrix(std::move(other.projectionMatrix)),
    //  position(std::move(other.position)),
    //  direction(std::move(other.direction)),
    //  up(std::move(other.up)),
    //  right(std::move(other.right)),
    //  viewport(std::move(other.viewport))
    //{
    //}
    //
    //Camera(const Camera& other)
    //: viewMatrix(other.viewMatrix),
    //  projectionMatrix(other.projectionMatrix),
    //  position(other.position),
    //  direction(other.direction),
    //  up(other.up),
    //  right(other.right),
    //  viewport(other.viewport)
    //{
    //}
    //
    //Camera operator=(Camera&& other) noexcept
    //{
    //  return Camera(other);
    //}
    //
    //Camera operator=(const Camera& other)
    //{
    //  return Camera(other);
    //}
  };
}

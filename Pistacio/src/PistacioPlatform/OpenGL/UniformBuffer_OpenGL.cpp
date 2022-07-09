#include "pch.h"
#include "UniformBuffer_OpenGL.h"

namespace Pistacio
{



  UniformBuffer_OpenGL::UniformBuffer_OpenGL(uint32_t size, uint32_t binding)
  {
    glGenBuffers(1, &RendererId);

    glBindBuffer(GL_UNIFORM_BUFFER, RendererId);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, binding, RendererId);
  }

  UniformBuffer_OpenGL::~UniformBuffer_OpenGL()
  {
    glDeleteBuffers(1, &RendererId);
  }

  void UniformBuffer_OpenGL::SetData(const void* data, uint32_t size, uint32_t offset) const
  {
    glNamedBufferSubData(RendererId, offset, size, data);
  }

}
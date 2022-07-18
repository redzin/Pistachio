#include "pch.h"
#include "UniformBuffer_OpenGL.h"

namespace Pistacio
{

  UniformBuffer_OpenGL::UniformBuffer_OpenGL(uint32_t size, uint32_t binding) : m_Size(size)
  {
    glGenBuffers(1, &m_RendererId);

    glBindBuffer(GL_UNIFORM_BUFFER, m_RendererId);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererId);
  }

  UniformBuffer_OpenGL::~UniformBuffer_OpenGL()
  {
    glDeleteBuffers(1, &m_RendererId);
  }

  const RendererID UniformBuffer_OpenGL::GetRendererID() const
  {
    return m_RendererId;
  }

  void UniformBuffer_OpenGL::SetData(const void* data, uint32_t size, uint32_t offset) const
  {
    glNamedBufferSubData(m_RendererId, offset, size, data);
  }

}
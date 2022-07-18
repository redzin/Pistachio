#include "pch.h"
#include "Buffer_OpenGL.h"

namespace Pistacio
{

  VertexBuffer_OpenGL::VertexBuffer_OpenGL(float* vertices, uint32_t size)
  {
    glCreateBuffers(1, &m_RendererId);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  }

  VertexBuffer_OpenGL::~VertexBuffer_OpenGL()
  {
    glDeleteBuffers(1, &m_RendererId);
  }

  //void VertexBuffer_OpenGL::Bind() const
  //{
  //  glBindBuffer(GL_ARRAY_BUFFER, RendererId);
  //}
  //
  //void VertexBuffer_OpenGL::Unbind() const
  //{
  //  glBindBuffer(GL_ARRAY_BUFFER, 0);
  //}

  void VertexBuffer_OpenGL::SetLayout(BufferLayout layout)
  {
    this->m_Layout = layout;
  }

  const BufferLayout& VertexBuffer_OpenGL::GetLayout() const
  {
    return m_Layout;
  }

  const RendererID VertexBuffer_OpenGL::GetRendererID() const
  {
    return m_RendererId;
  }
  
  IndexBuffer_OpenGL::IndexBuffer_OpenGL(uint32_t* indices, uint32_t count) : m_Count(count)
  {
    glCreateBuffers(1, &m_RendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
  }

  IndexBuffer_OpenGL::~IndexBuffer_OpenGL()
  {
    glDeleteBuffers(1, &m_RendererId);
  }

  uint32_t IndexBuffer_OpenGL::GetCount() const
  {
    return m_Count;
  }

  const RendererID IndexBuffer_OpenGL::GetRendererID() const
  {
    return m_RendererId;
  }

  //void IndexBuffer_OpenGL::Bind() const
  //{
  //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererId);
  //}
  //
  //void IndexBuffer_OpenGL::Unbind() const
  //{
  //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  //}

}
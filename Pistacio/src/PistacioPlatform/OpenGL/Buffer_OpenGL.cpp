#include "pch.h"
#include "Buffer_OpenGL.h"

namespace Pistacio
{

  VertexBuffer_OpenGL::VertexBuffer_OpenGL(float* vertices, uint32_t size)
  {
    glCreateBuffers(1, &RendererId);
    glBindBuffer(GL_ARRAY_BUFFER, RendererId);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  }

  VertexBuffer_OpenGL::~VertexBuffer_OpenGL()
  {
    glDeleteBuffers(1, &RendererId);
  }

  void VertexBuffer_OpenGL::Bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, RendererId);
  }

  void VertexBuffer_OpenGL::Unbind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void VertexBuffer_OpenGL::SetLayout(BufferLayout layout)
  {
    this->layout = layout;
  }

  const BufferLayout& VertexBuffer_OpenGL::GetLayout() const
  {
    return layout;
  }
  
  IndexBuffer_OpenGL::IndexBuffer_OpenGL(uint32_t* indices, uint32_t count) : count(count)
  {
    glCreateBuffers(1, &RendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
  }

  IndexBuffer_OpenGL::~IndexBuffer_OpenGL()
  {
    glDeleteBuffers(1, &RendererId);
  }

  void IndexBuffer_OpenGL::Bind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererId);
  }

  void IndexBuffer_OpenGL::Unbind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

}
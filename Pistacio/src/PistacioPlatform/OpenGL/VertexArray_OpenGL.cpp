#include "pch.h"
#include "VertexArray_OpenGL.h"

#include "PistacioPlatform/OpenGL/Buffer_OpenGL.h"

namespace Pistacio
{
  VertexArray_OpenGL::VertexArray_OpenGL()
  {
    glCreateVertexArrays(1, &m_RendererId);
  }

  const RendererID VertexArray_OpenGL::GetRendererID() const
  {
    return m_RendererId;
  }

  void VertexArray_OpenGL::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
  {

    glBindVertexArray(m_RendererId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetRendererID());

    int index = 0;
    BufferLayout layout = vertexBuffer->GetLayout();
    for (const auto& entry : layout)
    {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(
        index,
        entry.GetComponentCount(),
        ShaderDataTypeToOpenGLBaseType(entry.type),
        entry.normalized ? GL_TRUE : GL_FALSE,
        layout.GetStride(),
        (const void*)entry.offset
      );
      index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);

  }

  void VertexArray_OpenGL::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
  {

    glBindVertexArray(m_RendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetRendererID());
    this->m_IndexBuffer = indexBuffer;

  }

  Ref<IndexBuffer> VertexArray_OpenGL::GetIndexBuffer()
  {
      return m_IndexBuffer;
  }

  const std::vector<Ref<VertexBuffer>>& VertexArray_OpenGL::GetVertexBuffers()
  {
    return m_VertexBuffers;
  }

}

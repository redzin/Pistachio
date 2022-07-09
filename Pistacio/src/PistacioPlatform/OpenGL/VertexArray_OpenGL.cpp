#include "pch.h"
#include "VertexArray_OpenGL.h"

#include "PistacioPlatform/OpenGL/Buffer_OpenGL.h"

namespace Pistacio
{
  VertexArray_OpenGL::VertexArray_OpenGL()
  {
    glCreateVertexArrays(1, &RendererId);
  }

  void VertexArray_OpenGL::Bind() const
  {
    glBindVertexArray(RendererId);
  }

  void VertexArray_OpenGL::Unbind() const
  {
    glBindVertexArray(0);
  }

  void VertexArray_OpenGL::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
  {

    glBindVertexArray(RendererId);
    vertexBuffer->Bind();

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

    vertexBuffers.push_back(vertexBuffer);

  }

  void VertexArray_OpenGL::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
  {

    glBindVertexArray(RendererId);
    indexBuffer->Bind();
    this->indexBuffer = indexBuffer;

  }

  std::shared_ptr<IndexBuffer> VertexArray_OpenGL::GetIndexBuffer()
  {
      return indexBuffer;
  }

  const std::vector<std::shared_ptr<VertexBuffer>>& VertexArray_OpenGL::GetVertexBuffers()
  {
    return vertexBuffers;
  }

}

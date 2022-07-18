#pragma once
#include "pch.h"
#include "Pistacio/Rendering/VertexArray.h"

namespace Pistacio
{

  static GLenum ShaderDataTypeToOpenGLBaseType(BufferDataType type)
  {
    {
      switch (type)
      {
        case BufferDataType::Bool:    return GL_BOOL;
        case BufferDataType::Int:     return GL_INT;
        case BufferDataType::Int2:    return GL_INT;
        case BufferDataType::Int3:    return GL_INT;
        case BufferDataType::Int4:    return GL_INT;
        case BufferDataType::Float:   return GL_FLOAT;
        case BufferDataType::Float2:  return GL_FLOAT;
        case BufferDataType::Float3:  return GL_FLOAT;
        case BufferDataType::Float4:  return GL_FLOAT;
        case BufferDataType::Mat3:    return GL_FLOAT;
        case BufferDataType::Mat4:    return GL_FLOAT;
      }
    }
  }

  class VertexArray_OpenGL : public VertexArray
  {
  public:
    VertexArray_OpenGL();
    virtual ~VertexArray_OpenGL() {}
    const RendererID GetRendererID() const override;
    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
    Ref<IndexBuffer> GetIndexBuffer();
    const std::vector<Ref<VertexBuffer>>& GetVertexBuffers();

  private:
    RendererID m_RendererId;
    std::vector<Ref<VertexBuffer>> m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
  };

}


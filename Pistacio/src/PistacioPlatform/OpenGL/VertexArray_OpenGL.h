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
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
    virtual std::shared_ptr<IndexBuffer> GetIndexBuffer();
    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers();

  private:
    unsigned int RendererId;
    std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
    std::shared_ptr<IndexBuffer> indexBuffer;
  };

}


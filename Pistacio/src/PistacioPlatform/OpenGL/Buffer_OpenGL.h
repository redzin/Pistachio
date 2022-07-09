#pragma once
#include "pch.h"

#include "Pistacio/Rendering/Buffer.h"

namespace Pistacio
{
  class VertexBuffer_OpenGL : public VertexBuffer
  {
  public:
    VertexBuffer_OpenGL(float* vertices, uint32_t size);
    virtual ~VertexBuffer_OpenGL();
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SetLayout(BufferLayout layout) override;
    virtual const BufferLayout& GetLayout() const override;

  private:
    unsigned int RendererId;
    BufferLayout layout;
  };

  class IndexBuffer_OpenGL : public IndexBuffer
  {
  public:
    IndexBuffer_OpenGL(uint32_t* indices, uint32_t count);
    virtual ~IndexBuffer_OpenGL();
    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual uint32_t GetCount() const override { return count; }
  private:
    uint32_t count;
    unsigned int RendererId;
  };
}


#pragma once
#include "pch.h"

#include "Pistacio/Rendering/Buffer.h"

namespace Pistacio
{
  class VertexBuffer_OpenGL : public VertexBuffer
  {
  public:
    VertexBuffer_OpenGL(float* vertices, uint32_t size);
    ~VertexBuffer_OpenGL();
    void SetLayout(BufferLayout layout) override;
    const BufferLayout& GetLayout() const override;
    const RendererID GetRendererID() const override;

  private:
    RendererID m_RendererId;
    BufferLayout m_Layout;
  };

  class IndexBuffer_OpenGL : public IndexBuffer
  {
  public:
    IndexBuffer_OpenGL(uint32_t* indices, uint32_t count);
    ~IndexBuffer_OpenGL();
    uint32_t GetCount() const override;
    const RendererID GetRendererID() const override;
  private:
    uint32_t m_Count;
    RendererID m_RendererId;
  };
}


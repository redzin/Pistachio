#pragma once
#include "pch.h"
#include "Pistacio/Rendering/Buffer.h"

namespace Pistacio 
{
  class VertexArray
  {
  public:
    virtual const RendererID GetRendererID() const = 0;
    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
    virtual Ref<IndexBuffer> GetIndexBuffer() = 0;
    virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() = 0;

    static Ref<VertexArray> Create();
  };
}


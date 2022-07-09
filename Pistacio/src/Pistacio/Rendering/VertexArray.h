#pragma once
#include "pch.h"
#include "Pistacio/Rendering/Buffer.h"

namespace Pistacio 
{
  class VertexArray
  {
  public:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
    virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() = 0;
    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() = 0;

    static VertexArray* Create();
  };
}


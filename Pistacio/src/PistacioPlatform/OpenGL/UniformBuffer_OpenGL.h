#pragma once

#include "pch.h"

#include "Pistacio/Rendering/UniformBuffer.h"

namespace Pistacio
{

  class UniformBuffer_OpenGL : public UniformBuffer
  {
  public:
    UniformBuffer_OpenGL(uint32_t size, uint32_t binding);
    ~UniformBuffer_OpenGL();
    const RendererID GetRendererID() const override;
    void SetData(const void* data, uint32_t size, uint32_t offset) const override;
  private:
    RendererID m_RendererId;
    uint32_t m_Size;
  };

}

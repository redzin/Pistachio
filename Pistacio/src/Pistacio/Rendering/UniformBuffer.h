#pragma once
#include "pch.h"

#include "Buffer.h"

namespace Pistacio
{

  class UniformBuffer
  {
  public:
    virtual ~UniformBuffer() = default;

    virtual void SetData(const void* data, uint32_t size, uint32_t offset) const = 0;

    static UniformBuffer* Create(uint32_t size, uint32_t binding);
  };

}


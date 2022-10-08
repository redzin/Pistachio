#pragma once
#include "pch.h"

namespace Pistachio
{

  struct RenderableSpriteComponent
  {
  public:
    RenderableSpriteComponent(Device& device, uint32_t spriteCount) : m_SpriteCount(spriteCount)
    {

      BufferDescriptor spriteTexIndexAttributeDesc;
      spriteTexIndexAttributeDesc.AttributeDivisor = 1;
      spriteTexIndexAttributeDesc.Size = 4 * spriteCount;
      spriteTexIndexAttributeDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
      TexCoordIndexBuffer = device.CreateBuffer(spriteTexIndexAttributeDesc);

      BufferDescriptor spriteTransformAttributeDesc;
      spriteTransformAttributeDesc.AttributeDivisor = 1;
      spriteTransformAttributeDesc.Size = 64 * spriteCount;
      spriteTransformAttributeDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
      TransformBuffer = device.CreateBuffer(spriteTransformAttributeDesc);

    }

    uint32_t Count() { return m_SpriteCount; }

    void UpdateAttributes(std::vector<uint32_t> samplerIndices, std::vector<glm::mat4> transforms)
    {
      uint32_t* texIdxGpuPtr = (uint32_t*)TexCoordIndexBuffer->MemoryPtr;
      glm::mat4* transformGpuPtr = (glm::mat4*)TransformBuffer->MemoryPtr;
    
      memcpy(texIdxGpuPtr, samplerIndices.data(), samplerIndices.size() * sizeof(uint32_t));
      memcpy(transformGpuPtr, transforms.data(), transforms.size() * sizeof(glm::mat4));
    }

    Ref<Buffer> TexCoordIndexBuffer;
    Ref<Buffer> TransformBuffer;

  private:
    uint32_t m_SpriteCount;
  };

}



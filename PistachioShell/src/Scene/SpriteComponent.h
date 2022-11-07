#pragma once
#include "pch.h"
#include "Camera/Camera.h"

namespace Pistachio
{

  struct SpritePassData
  {
    RenderPassState RenderPassState;
    Ref<RenderPass> RenderPass;
    Ref<Buffer> IndexBuffer;
    Ref<Buffer> PositionBuffer;
    Ref<Buffer> TexCoordBuffer;
    Ref<Attachment> ColorAttachment;
    Ref<Attachment> DepthAttachment;
    Ref<Sampler> Sampler;
  };
  
  SpritePassData CreateSpritePass(Device& device,
    RenderGraph& renderGraph,
    std::unordered_map<std::string, uint32_t> textureMap,
    uint32_t maxSpriteCount,
    uint32_t samplerWidth,
    uint32_t samplerHeight);
  void ResizeAttachments(SpritePassData& spritePassData, Device& device, uint32_t viewportWidth, uint32_t viewportHeight);
  void BeginFrame(SpritePassData& spritePassData, const glm::vec4& clearColor, Camera& camera);

  struct RenderableSpriteComponent
  {
  public:
    RenderableSpriteComponent(Device& device, uint32_t spriteCount);

    ~RenderableSpriteComponent();

    uint32_t Count() const { return m_SpriteCount; }

    Ref<Buffer> TexCoordIndexBuffer;
    Ref<Buffer> TransformBuffer;

  private:
    uint32_t m_SpriteCount;
  };

  void Draw(const RenderableSpriteComponent& spriteComponent, Camera& camera, Device& device, SpritePassData& spritePassData);

}



#pragma once

#include "pch.h"
#include "../Scene/Scene.h"
#include "../Scene/SpriteComponent.h"
#include "../Scene/SceneComponents.h"
#include "../Scene/Camera/Camera.h"

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
  SpritePassData CreateSpritePass(Device& device, RenderGraph& renderGraph, std::unordered_map<std::string, uint32_t> textureMap, uint32_t maxSpriteCount, uint32_t samplerWidth, uint32_t samplerHeight);
  void ResizeAttachments(SpritePassData& spritePassData, Device& device, uint32_t viewportWidth, uint32_t viewportHeight);
  void RecordRenderCommands(SpritePassData& spritePassData, Device& device, Scene& scene, Camera& camera, glm::vec4 clearColor);

  struct PBRPassData
  {
    RenderPassState RenderPassState;
    Ref<RenderPass> RenderPass;
    Ref<Buffer> ModelUniformBuffer;
    Ref<Attachment> ColorAttachment;
    Ref<Attachment> DepthAttachment;
  };
  PBRPassData CreatePBRPass(Device& device, RenderGraph& renderGraph);
  void ResizeAttachments(PBRPassData& pbrPassData, Device& device, uint32_t viewportWidth, uint32_t viewportHeight);
  void RecordRenderCommands(PBRPassData& pbrPassData, Device& device, Scene& scene, Camera& camera, glm::vec4 clearColor);


  enum class SceneRenderPassType
  {
    SpritePass
  };

  class SceneRenderer
  {
  public:
    SceneRenderer() = default;
    ~SceneRenderer() = default;
    
    void Render(Device& device, Scene& scene, Camera& camera, uint32_t viewportWidth, uint32_t viewportHeight, glm::vec4 clearColor);
    std::map<std::string, Ref<Attachment>> GetDisplayReadyAttachments();

    void Init(
      Device& device,
      std::unordered_map<std::string, uint32_t> textureMap,
      uint32_t maxSpriteCount,
      uint32_t samplerWidth,
      uint32_t samplerHeight
    );

  private:
    SpritePassData m_SpritePassData;
    PBRPassData m_PBRPassData;
    RenderGraph m_RenderGraph;
  };

}



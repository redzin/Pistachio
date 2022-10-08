#pragma once

#include "pch.h"
#include "../SceneComponents/SpriteComponent.h"
#include "../SceneComponents/SceneComponents.h"

namespace Pistachio
{

  class SceneRenderer
  {
  public:
    SceneRenderer(Device& device);
    ~SceneRenderer() = default;
    
    void Render(Device& device, Scene& scene, Camera& camera, uint32_t viewportWidth, uint32_t viewportHeight, glm::vec4 clearColor);
    std::map<std::string, Ref<Attachment>> GetDisplayReadyAttachments();

    void InitSpriteRenderer(
      Device& device,
      std::unordered_map<std::string, uint32_t> textureMap,
      uint32_t maxSpriteCount,
      uint32_t samplerWidth,
      uint32_t samplerHeight
    );

    enum class SceneRenderPass
    {
      SpritePass
    };


  private:
    void ResizeAttachments(Device& device, uint32_t viewport_width, uint32_t viewport_height);

  private: // Sprite Pass
    void ResizeSpritePassAttachments(Device& device, uint32_t viewportWidth, uint32_t viewportHeight);
    void RecordSpritePassRenderCommands(Device& device, Scene& scene, Camera& camera, glm::vec4 clearColor);
    
  private:
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

    SpritePassData m_SpritePassData;

  private:
    Ref<Buffer> PerFrameUniformBuffer;

  private:

    RenderGraph m_RenderGraph;
  };

}



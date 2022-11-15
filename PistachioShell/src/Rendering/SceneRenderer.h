#pragma once

#include "pch.h"
#include "../Scene/Scene.h"
#include "../Scene/SpriteComponent.h"
#include "../Scene/SceneComponents.h"
#include "../Scene/Camera/Camera.h"

namespace Pistachio
{
  class SceneRenderer
  {
  public:
    SceneRenderer() = default;
    ~SceneRenderer() = default;
    
    void Init(Device& device, std::unordered_map<std::string, uint32_t> textureMap, uint32_t maxSpriteCount, uint32_t samplerWidth, uint32_t samplerHeight);
    void Render(Device& device, Scene& scene, Camera& camera, uint32_t viewportWidth, uint32_t viewportHeight, PBRShaderOverrides& shaderOverrides, glm::vec4 clearColor);
    std::map<std::string, Ref<Attachment>> GetDisplayReadyAttachments();

  private:
    RenderGraph m_RenderGraph;
    SpritePassData m_SpritePassData;

    PBRPassData m_PBRPassData;
  };

}



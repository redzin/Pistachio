#include "SceneRenderer.h"

namespace Pistachio
{

  SceneRenderer::SceneRenderer(Device& device)
  {
  }

  std::map<std::string, Ref<Attachment>> SceneRenderer::GetDisplayReadyAttachments()
  {
    std::map<std::string, Ref<Attachment>> attachments;

    attachments["SpritePass_ColorAttachment"] = m_SpritePassData.ColorAttachment;
    attachments["SpritePass_DepthAttachment"] = m_SpritePassData.DepthAttachment;

    return attachments;
  }

  void SceneRenderer::ResizeAttachments(Device& device, uint32_t viewportWidth, uint32_t viewportHeight)
  {
    ResizeSpritePassAttachments(device, viewportWidth, viewportHeight);
  }

  void SceneRenderer::ResizeSpritePassAttachments(Device& device, uint32_t viewportWidth, uint32_t viewportHeight)
  {
    if (!m_SpritePassData.ColorAttachment || m_SpritePassData.ColorAttachment->AttachmentDescriptor.TextureDescriptor.SizeX != viewportWidth || m_SpritePassData.ColorAttachment->AttachmentDescriptor.TextureDescriptor.SizeY != viewportHeight)
    {
      AttachmentDescriptor colorAttachmentDesc{};
      colorAttachmentDesc.TextureDescriptor.SizeX = viewportWidth;
      colorAttachmentDesc.TextureDescriptor.SizeY = viewportHeight;
      colorAttachmentDesc.TextureDescriptor.InternalFormat = INTERNAL_FORMAT_RGB8;
      colorAttachmentDesc.TextureDescriptor.Samples = 1;
      colorAttachmentDesc.AttachmentPoint = COLOR_ATTACHMENT_0;

      AttachmentDescriptor depthAttachmentDesc{};
      depthAttachmentDesc.TextureDescriptor.SizeX = colorAttachmentDesc.TextureDescriptor.SizeX;
      depthAttachmentDesc.TextureDescriptor.SizeY = colorAttachmentDesc.TextureDescriptor.SizeY;
      depthAttachmentDesc.TextureDescriptor.InternalFormat = INTERNAL_FORMAT_DEPTH_COMPONENT_32;
      depthAttachmentDesc.TextureDescriptor.Samples = 1;
      depthAttachmentDesc.AttachmentPoint = DEPTH_ATTACHMENT;

      m_SpritePassData.ColorAttachment = device.CreateAttachment(colorAttachmentDesc);
      m_SpritePassData.DepthAttachment = device.CreateAttachment(depthAttachmentDesc);

      m_SpritePassData.RenderPass->AddAttachmentOutput(m_SpritePassData.ColorAttachment);
      m_SpritePassData.RenderPass->AddAttachmentOutput(m_SpritePassData.DepthAttachment);
    }
  }

  void SceneRenderer::Render(Device& device, Scene& scene, Camera& camera, uint32_t viewportWidth, uint32_t viewportHeight, glm::vec4 clearColor)
  {
    ResizeAttachments(device, viewportWidth, viewportHeight);

    RecordSpritePassRenderCommands(device, scene, camera, clearColor);

    m_RenderGraph.Render(device, viewportWidth, viewportHeight);
  }

  void SceneRenderer::RecordSpritePassRenderCommands(Device& device, Scene& scene, Camera& camera, glm::vec4 clearColor)
  {
    //m_RenderGraph = RenderGraph();
    m_SpritePassData.RenderPass->SetRenderState(m_SpritePassData.RenderPassState);
    m_SpritePassData.RenderPass->SetShaderProgram("assets/shaders/SpriteInstanced.glsl");

    m_SpritePassData.RenderPass->RecordCommandBuffer([&clearColor, &camera, this](Device& device, RenderingAPI& api)
      {
        api.SetClearColor(clearColor);
        api.SetClearDepth(1.0f);
        api.Clear(COLOR_BUFFER | DEPTH_BUFFER);
        api.SetBufferUniformBinding(camera.ViewProjectionBuffer->RendererID, 0);
      }
    );

    std::function<void(RenderableSpriteComponent&, EntityID&)> RenderSprites = [this, &device, &camera](RenderableSpriteComponent& spriteComponent, EntityID& spriteId)
    {
      m_SpritePassData.RenderPass->RecordCommandBuffer([&device, &camera, &spriteComponent, this](Device& device, RenderingAPI& api)
        {
          AttributeLayoutDescriptor attributeDesc;
          attributeDesc.push_back({ m_SpritePassData.PositionBuffer, 0, 1, { {BufferDataType::Float3, "a_Position"} } });
          attributeDesc.push_back({ m_SpritePassData.TexCoordBuffer, 1, 1, { {BufferDataType::Float2, "a_TexCoord"} } });
          attributeDesc.push_back({ spriteComponent.TexCoordIndexBuffer, 2, 1, { {BufferDataType::UnsignedInt, "a_TexIndex"} } });
          attributeDesc.push_back({ spriteComponent.TransformBuffer, 3, 4, { {BufferDataType::Mat4, "a_Transform"} } });
          Ref<AttributeLayout> attributebuteLayout = device.RequestAttributeLayout(attributeDesc, m_SpritePassData.IndexBuffer->RendererID);

          api.BindSampler(m_SpritePassData.Sampler->RendererID, 0);
          api.DrawIndexedInstanced(attributebuteLayout->RendererID, 6, spriteComponent.Count(), PRIMITIVE_TRIANGLES);
        }
      );
    };

    auto spriteView = scene.GetGroup<>(Scene::GroupGet<RenderableSpriteComponent>, Scene::GroupExclude<TransparentRenderableComponent>);
    for (EntityID spriteId : spriteView)
    {
      RenderableSpriteComponent& spriteComponent = spriteView.get<RenderableSpriteComponent>(spriteId);
      RenderSprites(spriteComponent, spriteId);
    }

    auto transparentSpriteView = scene.GetView<RenderableSpriteComponent, TransparentRenderableComponent>();
    for (EntityID spriteId : transparentSpriteView)
    {
      RenderableSpriteComponent& spriteComponent = transparentSpriteView.get<RenderableSpriteComponent>(spriteId);
      RenderSprites(spriteComponent, spriteId);
    }
  }

  void SceneRenderer::InitSpriteRenderer(Device& device,
                                      std::unordered_map<std::string, uint32_t> textureMap,
                                      uint32_t maxSpriteCount,
                                      uint32_t samplerWidth,
                                      uint32_t samplerHeight)
  {
    m_SpritePassData.RenderPass = CreateRef<RenderPass>("Sprite Pass");
    m_RenderGraph.AddPass(m_SpritePassData.RenderPass);

    m_SpritePassData.RenderPassState.Capabilities = { Capability::DepthTest, Capability::Blend};
    m_SpritePassData.RenderPassState.DepthFunction = DepthFunction::LEqual;
    m_SpritePassData.RenderPassState.DepthMask = true;
    m_SpritePassData.RenderPassState.BlendState = BlendState{ BlendFunction::SrcAlpha, BlendFunction::OneMinusSrcAlpha };
    
    float vertexPositions[3 * 4] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
    };
    float vertexTexCoords[2 * 4] = {
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f
    };
    uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
    
    BufferDescriptor spritePositionBufferDesc;
    spritePositionBufferDesc.Size = sizeof(vertexPositions);
    spritePositionBufferDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
    m_SpritePassData.PositionBuffer = device.CreateBuffer(spritePositionBufferDesc, vertexPositions);
    
    BufferDescriptor spriteTexCoordBufferDesc;
    spriteTexCoordBufferDesc.Size = sizeof(vertexTexCoords);
    spriteTexCoordBufferDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
    m_SpritePassData.TexCoordBuffer = device.CreateBuffer(spriteTexCoordBufferDesc, vertexTexCoords);
    
    BufferDescriptor spriteIndexBufferDesc;
    spriteIndexBufferDesc.Size = sizeof(indices);
    spriteIndexBufferDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
    m_SpritePassData.IndexBuffer = device.CreateBuffer(spriteIndexBufferDesc, indices);
    
    SamplerDescriptor samplerDesc;
    samplerDesc.TextureDescriptor.InternalFormat = INTERNAL_FORMAT_RGBA8;
    samplerDesc.TextureDescriptor.Layers = 32;
    samplerDesc.TextureDescriptor.SizeX = samplerWidth;
    samplerDesc.TextureDescriptor.SizeY = samplerHeight;
    samplerDesc.TextureDescriptor.Type = TEXTURE_2D_ARRAY;
    m_SpritePassData.Sampler = device.CreateSampler(samplerDesc);
  
    for (const auto& [path, index] : textureMap)
    {
      Image img = LoadImageFromFile(path);
      device.UploadSamplerData(m_SpritePassData.Sampler, img, index);
    }

  }
}

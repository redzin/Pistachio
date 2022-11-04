#include "SceneRenderer.h"

namespace Pistachio
{
  SpritePassData CreateSpritePass(Device& device,
    RenderGraph& renderGraph,
    std::unordered_map<std::string, uint32_t> textureMap,
    uint32_t maxSpriteCount,
    uint32_t samplerWidth,
    uint32_t samplerHeight)
  {
    SpritePassData spritePassData;

    spritePassData.RenderPass = CreateRef<RenderPass>("Sprite Pass");
    renderGraph.AddPass(spritePassData.RenderPass);

    spritePassData.RenderPassState.Capabilities = { Capability::DepthTest, Capability::Blend };
    spritePassData.RenderPassState.DepthFunction = DepthFunction::LEqual;
    spritePassData.RenderPassState.DepthMask = true;
    spritePassData.RenderPassState.BlendState = BlendState{ BlendFunction::SrcAlpha, BlendFunction::OneMinusSrcAlpha };

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
    spritePassData.PositionBuffer = device.CreateBuffer(spritePositionBufferDesc, vertexPositions);

    BufferDescriptor spriteTexCoordBufferDesc;
    spriteTexCoordBufferDesc.Size = sizeof(vertexTexCoords);
    spriteTexCoordBufferDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
    spritePassData.TexCoordBuffer = device.CreateBuffer(spriteTexCoordBufferDesc, vertexTexCoords);

    BufferDescriptor spriteIndexBufferDesc;
    spriteIndexBufferDesc.Size = sizeof(indices);
    spriteIndexBufferDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
    spritePassData.IndexBuffer = device.CreateBuffer(spriteIndexBufferDesc, indices);

    SamplerDescriptor samplerDesc;
    samplerDesc.TextureDescriptor.InternalFormat = INTERNAL_FORMAT_RGBA8;
    samplerDesc.TextureDescriptor.Layers = 32;
    samplerDesc.TextureDescriptor.SizeX = samplerWidth;
    samplerDesc.TextureDescriptor.SizeY = samplerHeight;
    samplerDesc.TextureDescriptor.Type = TEXTURE_2D_ARRAY;
    spritePassData.Sampler = device.CreateSampler(samplerDesc);

    for (const auto& [path, index] : textureMap)
    {
      Image img = LoadImageFromFile(path);
      device.UploadSamplerData(spritePassData.Sampler, img, index);
    }

    return spritePassData;
  }

  void ResizeAttachments(SpritePassData& spritePassData, Device& device, uint32_t viewportWidth, uint32_t viewportHeight)
  {
    if (!spritePassData.ColorAttachment || spritePassData.ColorAttachment->AttachmentDescriptor.TextureDescriptor.SizeX != viewportWidth || spritePassData.ColorAttachment->AttachmentDescriptor.TextureDescriptor.SizeY != viewportHeight)
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

      spritePassData.ColorAttachment = device.CreateAttachment(colorAttachmentDesc);
      spritePassData.DepthAttachment = device.CreateAttachment(depthAttachmentDesc);

      spritePassData.RenderPass->ClearAttachmentOutputs();
      spritePassData.RenderPass->AddAttachmentOutput(spritePassData.ColorAttachment);
      spritePassData.RenderPass->AddAttachmentOutput(spritePassData.DepthAttachment);
    }
  }

  void RecordRenderCommands(SpritePassData& spritePassData, Device& device, Scene& scene, Camera& camera, glm::vec4 clearColor)
  {
    spritePassData.RenderPass->SetRenderState(spritePassData.RenderPassState);
    spritePassData.RenderPass->SetShaderProgram("assets/shaders/SpriteInstanced.glsl");

    spritePassData.RenderPass->RecordCommandBuffer([&clearColor, &camera](Device& device, RenderingAPI& api)
      {
        api.SetClearColor(clearColor);
        api.SetClearDepth(1.0f);
        api.Clear(COLOR_BUFFER | DEPTH_BUFFER);
        api.SetBufferUniformBinding(camera.GetBuffer()->RendererID, 0);
      }
    );

    auto spriteView = scene.GetGroup<>(Scene::GroupGet<RenderableSpriteComponent>, Scene::GroupExclude<TransparentRenderableComponent>);
    for (EntityID spriteId : spriteView)
    {
      RenderableSpriteComponent& spriteComponent = spriteView.get<RenderableSpriteComponent>(spriteId);

      AttributeLayoutDescriptor attributeDesc;
      attributeDesc.push_back({ spritePassData.PositionBuffer, 0, 1, { {BufferDataType::Float3, "a_Position"} } });
      attributeDesc.push_back({ spritePassData.TexCoordBuffer, 1, 1, { {BufferDataType::Float2, "a_TexCoord"} } });
      attributeDesc.push_back({ spriteComponent.TexCoordIndexBuffer, 2, 1, { {BufferDataType::UnsignedInt, "a_TexIndex"} } });
      attributeDesc.push_back({ spriteComponent.TransformBuffer, 3, 4, { {BufferDataType::Mat4, "a_Transform"} } });
      AttributeLayout& attributebuteLayout = device.RequestAttributeLayout(attributeDesc, spritePassData.IndexBuffer->RendererID);

      uint32_t count = spriteComponent.Count();
      RendererID samplerId = spritePassData.Sampler->RendererID;
      RendererID vao = attributebuteLayout.RendererID;

      spritePassData.RenderPass->RecordCommandBuffer([&device, &camera, count, samplerId, vao](Device& device, RenderingAPI& api)
        {
          api.BindSampler(samplerId, 0);
          api.DrawIndexedInstanced(vao, 6, count, PRIMITIVE_TRIANGLES);
        }
      );
    }

    auto transparentSpriteView = scene.GetView<RenderableSpriteComponent, TransparentRenderableComponent>();
    for (EntityID spriteId : transparentSpriteView)
    {
      RenderableSpriteComponent& spriteComponent = transparentSpriteView.get<RenderableSpriteComponent>(spriteId);

      AttributeLayoutDescriptor attributeDesc;
      attributeDesc.push_back({ spritePassData.PositionBuffer, 0, 1, { {BufferDataType::Float3, "a_Position"} } });
      attributeDesc.push_back({ spritePassData.TexCoordBuffer, 1, 1, { {BufferDataType::Float2, "a_TexCoord"} } });
      attributeDesc.push_back({ spriteComponent.TexCoordIndexBuffer, 2, 1, { {BufferDataType::UnsignedInt, "a_TexIndex"} } });
      attributeDesc.push_back({ spriteComponent.TransformBuffer, 3, 4, { {BufferDataType::Mat4, "a_Transform"} } });
      AttributeLayout& attributebuteLayout = device.RequestAttributeLayout(attributeDesc, spritePassData.IndexBuffer->RendererID);

      uint32_t count = spriteComponent.Count();
      RendererID samplerId = spritePassData.Sampler->RendererID;
      RendererID vao = attributebuteLayout.RendererID;

      spritePassData.RenderPass->RecordCommandBuffer([&device, &camera, count, samplerId, vao](Device& device, RenderingAPI& api)
        {
          api.BindSampler(samplerId, 0);
          api.DrawIndexedInstanced(vao, 6, count, PRIMITIVE_TRIANGLES);
        }
      );
    }
  }

  PBRPassData CreatePBRPass(Device& device, RenderGraph& renderGraph)
  {
    PBRPassData pbrPassData;

    pbrPassData.RenderPass = CreateRef<RenderPass>("PBR Pass");
    renderGraph.AddPass(pbrPassData.RenderPass);

    pbrPassData.RenderPassState.Capabilities = { Capability::DepthTest, Capability::Blend };
    pbrPassData.RenderPassState.DepthFunction = DepthFunction::LEqual;
    pbrPassData.RenderPassState.DepthMask = true;
    pbrPassData.RenderPassState.BlendState = BlendState{ BlendFunction::SrcAlpha, BlendFunction::OneMinusSrcAlpha };

    BufferDescriptor modelTransformBuffer;
    modelTransformBuffer.Size = sizeof(glm::mat4);
    glm::mat4 identity = glm::mat4(1.0f);
    pbrPassData.ModelUniformBuffer = device.CreateBuffer(modelTransformBuffer, &identity);

    return pbrPassData;
  }

  void ResizeAttachments(PBRPassData& pbrPassData, Device& device, uint32_t viewportWidth, uint32_t viewportHeight)
  {
    if (!pbrPassData.ColorAttachment || pbrPassData.ColorAttachment->AttachmentDescriptor.TextureDescriptor.SizeX != viewportWidth || pbrPassData.ColorAttachment->AttachmentDescriptor.TextureDescriptor.SizeY != viewportHeight)
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

      pbrPassData.ColorAttachment = device.CreateAttachment(colorAttachmentDesc);
      pbrPassData.DepthAttachment = device.CreateAttachment(depthAttachmentDesc);

      pbrPassData.RenderPass->ClearAttachmentOutputs();
      pbrPassData.RenderPass->AddAttachmentOutput(pbrPassData.ColorAttachment);
      pbrPassData.RenderPass->AddAttachmentOutput(pbrPassData.DepthAttachment);
    }
  }

  void RecordRenderCommands(PBRPassData& pbrPassData, Device& device, Scene& scene, Camera& camera, glm::vec4 clearColor)
  {
    pbrPassData.RenderPass->SetRenderState(pbrPassData.RenderPassState);
    pbrPassData.RenderPass->SetShaderProgram("assets/shaders/PBR.glsl");

    pbrPassData.RenderPass->RecordCommandBuffer([&clearColor, &camera](Device& device, RenderingAPI& api)
      {
        api.SetClearColor(clearColor);
        api.SetClearDepth(1.0f);
        api.Clear(COLOR_BUFFER | DEPTH_BUFFER);
        api.SetBufferUniformBinding(camera.GetBuffer()->RendererID, 0);
      }
    );

    auto modelsView = scene.GetView<Model, Transform>();
    for (EntityID modelId : modelsView)
    {
      Model& model = modelsView.get<Model>(modelId);
      Transform& transform = modelsView.get<Transform>(modelId);

      memcpy(pbrPassData.ModelUniformBuffer->MemoryPtr, &transform, sizeof(glm::mat4));

      for (const StaticMesh& mesh : model.Meshes)
      {
        AttributeLayoutDescriptor attributeDesc;
        attributeDesc.push_back({ mesh.PositionBuffer, 0, 1, { {mesh.PositionBuffer->Descriptor.DataType, "a_Position"} } });
        attributeDesc.push_back({ mesh.NormalBuffer, 1, 2, { {mesh.NormalBuffer->Descriptor.DataType, "a_Normal"} } });
        AttributeLayout& attributebuteLayout = device.RequestAttributeLayout(attributeDesc, mesh.IndexBuffer->RendererID);

        uint32_t count = mesh.GetIndexCount();
        RendererID vao = attributebuteLayout.RendererID;
        uint32_t indexBufferBaseType = ShaderDataTypeToOpenGLBaseType(mesh.IndexBuffer->Descriptor.DataType);
        RendererID modelUniformBuffer = pbrPassData.ModelUniformBuffer->RendererID;

        pbrPassData.RenderPass->RecordCommandBuffer([&camera, modelUniformBuffer, vao, count, indexBufferBaseType](Device& device, RenderingAPI& api)
          {
            api.SetBufferUniformBinding(modelUniformBuffer, 1);
            api.DrawIndexed(vao, count, PRIMITIVE_TRIANGLES, indexBufferBaseType);
          }
        );
      }

    }
  }

  std::map<std::string, Ref<Attachment>> SceneRenderer::GetDisplayReadyAttachments()
  {
    std::map<std::string, Ref<Attachment>> attachments;

    attachments["PBRPass_ColorAttachment"] = m_PBRPassData.ColorAttachment;
    attachments["PBRPass_DepthAttachment"] = m_PBRPassData.DepthAttachment;
    attachments["SpritePass_ColorAttachment"] = m_SpritePassData.ColorAttachment;
    attachments["SpritePass_DepthAttachment"] = m_SpritePassData.DepthAttachment;

    return attachments;
  }

  void SceneRenderer::Init(Device& device, std::unordered_map<std::string, uint32_t> textureMap, uint32_t maxSpriteCount, uint32_t samplerWidth, uint32_t samplerHeight)
  {

    m_SpritePassData = CreateSpritePass(device, m_RenderGraph, textureMap, maxSpriteCount, samplerWidth, samplerHeight);
    m_PBRPassData = CreatePBRPass(device, m_RenderGraph);
    
  }

  void SceneRenderer::Render(Device& device, Scene& scene, Camera& camera, uint32_t viewportWidth, uint32_t viewportHeight, glm::vec4 clearColor)
  {

    ResizeAttachments(m_SpritePassData, device, viewportWidth, viewportHeight);
    ResizeAttachments(m_PBRPassData, device, viewportWidth, viewportHeight);

    RecordRenderCommands(m_SpritePassData, device, scene, camera, clearColor);
    RecordRenderCommands(m_PBRPassData, device, scene, camera, clearColor);

    m_RenderGraph.Render(device, viewportWidth, viewportHeight);

  }

  
}

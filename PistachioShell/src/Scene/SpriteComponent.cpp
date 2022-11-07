#include "pch.h"
#include "SpriteComponent.h"

namespace Pistachio
{
  void Draw(const RenderableSpriteComponent& spriteComponent, Camera& camera, Device& device, SpritePassData& spritePassData)
  {
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

        camera.GetBuffer()->Lock();
      }
    );
  }

  SpritePassData CreateSpritePass(Device& device, RenderGraph& renderGraph, std::unordered_map<std::string, uint32_t> textureMap, uint32_t maxSpriteCount, uint32_t samplerWidth, uint32_t samplerHeight)
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

  void BeginFrame(SpritePassData& spritePassData, const glm::vec4& clearColor, Camera& camera)
  {
    spritePassData.RenderPass->SetRenderState(spritePassData.RenderPassState);
    spritePassData.RenderPass->SetShaderProgram("assets/shaders/SpriteInstanced.glsl");

    spritePassData.RenderPass->RecordCommandBuffer([&clearColor, &camera](Device& device, RenderingAPI& api)
      {
        api.SetClearColor(clearColor);
        api.SetClearDepth(1.0f);
        api.Clear(COLOR_BUFFER | DEPTH_BUFFER);
        api.BindBuffer(UNIFORM_BUFFER_TARGET, camera.GetBuffer()->RendererID, 0);
      }
    );
  }

  RenderableSpriteComponent::RenderableSpriteComponent(Device& device, uint32_t spriteCount)
    : m_SpriteCount(spriteCount)
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

  RenderableSpriteComponent::~RenderableSpriteComponent()
  {
    PSTC_TRACE("RenderableSpriteComponent destroyed!");
  }
}


#pragma once
#include <Pistacio.h>
#include "Pistacio/Rendering/RenderGraph.h"
#include <algorithm>
#include <execution>

using namespace Pistacio;

namespace Examples
{

  static void UpdateSpritesInstanced(Device& device, const RenderingAPI& renderingAPI, Scene& scene)
  {

    PersistentBuffer* textureIndexBuffer = device.RequestPersistentBuffer("SpriteTexIndexAttribute");
    PersistentBuffer* transformBuffer = device.RequestPersistentBuffer("SpriteTransformAttribute");

    auto spritesView = scene.GetGroup<>(
      Scene::GroupGet<SpriteRenderableComponent>, Scene::GroupExclude<TransparentRenderableComponent>
    );

    uint32_t instanceIndex = 0;
    for (EntityID id : spritesView)
    {
      auto& sprite = spritesView.get<SpriteRenderableComponent>(id);
      
      uint32_t* texIdxGpuPtr = (uint32_t*)textureIndexBuffer->MemoryPtr;
      texIdxGpuPtr[instanceIndex] = sprite.SamplerIndex;
    
      glm::mat4* transformGpuPtr = (glm::mat4*)transformBuffer->MemoryPtr;
      transformGpuPtr[instanceIndex] = sprite.Transform;

      instanceIndex++;
    }
  }

  static void UpdateSpritesInstanced_Transparent(Device& device, const RenderingAPI& renderingAPI, Scene& scene, std::unordered_map<std::string, uint32_t>& textureMap)
  {
  
    PersistentBuffer* textureIndexBuffer = device.RequestPersistentBuffer("SpriteTexIndexAttribute_Transparent");
    PersistentBuffer* transformBuffer = device.RequestPersistentBuffer("SpriteTransformAttribute_Transparent");
  
    auto spritesView = scene.GetView<SpriteRenderableComponent, TransparentRenderableComponent>();

    uint32_t instanceIndex = 0;
    for (EntityID id : spritesView)
    {
      auto& sprite = spritesView.get<SpriteRenderableComponent>(id);

      uint32_t* texIdxGpuPtr = (uint32_t*)textureIndexBuffer->MemoryPtr;
      texIdxGpuPtr[instanceIndex] = sprite.SamplerIndex;

      glm::mat4* transformGpuPtr = (glm::mat4*)transformBuffer->MemoryPtr;
      transformGpuPtr[instanceIndex] = sprite.Transform;

      instanceIndex++;
    }
  }

  static void UpdateInstancedSpriteConstants(Device& device, const RenderingAPI& renderingAPI, Camera camera)
  {
    glm::mat4 viewProjectionMatrix = camera.projectionMatrix * camera.viewMatrix;
    Buffer* perFrameBuffer = device.RequestBuffer("PerFrameBuffer");

    PSTC_ASSERT(perFrameBuffer, "Failed to get per frame ubo!");

    renderingAPI.UploadBufferData(perFrameBuffer->RendererID, perFrameBuffer->Descriptor, 0, &viewProjectionMatrix[0]);
    renderingAPI.SetBufferUniformBinding(perFrameBuffer->RendererID, 0);
  }

  static void RenderSpritesInstanced(Device& device, const RenderingAPI& renderingAPI, Scene& scene, const RendererID& vbo, Camera camera)
  {
    auto spriteView = scene.GetView<SpriteRenderableComponent>();
    const RendererID& samplerId = device.RequestSampler("SpriteInstancedSampler")->RendererID;
    renderingAPI.BindSampler(samplerId, 0);
    renderingAPI.DrawIndexedInstanced(vbo, 6, spriteView.size(), PRIMITIVE_TRIANGLES);
  }

  void AddTransparencySubpass(Device& device, RenderGraph& renderGraph, Scene& scene, Camera& camera, std::unordered_map<std::string, uint32_t> textureMap, uint32_t maxSpriteCount, uint32_t samplerWidth, uint32_t samplerHeight, glm::vec4& clear_color)
  {
    RenderPass& transparencyPass = *renderGraph.AddPass("SpritePassInstanced_TransparencySubpass");

    RenderPassState renderState({ Capability::DepthTest, Capability::Blend });
    renderState.DepthFunction = DepthFunction::LEqual;
    renderState.DepthMask = true;
    renderState.BlendState = BlendState{ BlendFunction::SrcAlpha, BlendFunction::OneMinusSrcAlpha };
    transparencyPass.SetRenderState(std::move(renderState));
    transparencyPass.SetShaderProgram("assets/shaders/SpriteInstanced.glsl");

    std::string spriteTexIndexAttributes_Transparency = "SpriteTexIndexAttribute_Transparent";
    PersistentBufferDescriptor spriteTexIndexAttributeDesc_Transparency;
    spriteTexIndexAttributeDesc_Transparency.AttributeDivisor = 1;
    spriteTexIndexAttributeDesc_Transparency.Size = 4 * maxSpriteCount;
    spriteTexIndexAttributeDesc_Transparency.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
    device.CreatePersistentBuffer(spriteTexIndexAttributes_Transparency, spriteTexIndexAttributeDesc_Transparency);

    std::string spriteTransformAttribute_Transparency = "SpriteTransformAttribute_Transparent";
    PersistentBufferDescriptor spriteTransformAttributeDesc_Transparency;
    spriteTransformAttributeDesc_Transparency.AttributeDivisor = 1;
    spriteTransformAttributeDesc_Transparency.Size = 64 * maxSpriteCount;
    spriteTransformAttributeDesc_Transparency.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
    device.CreatePersistentBuffer(spriteTransformAttribute_Transparency, spriteTransformAttributeDesc_Transparency);

    std::string spritePositionAttributes = "SpritePositionAttributes";
    std::string spriteTexCoordAttributes = "SpriteTexCoordAttributes";
    std::string spriteIndices = "SpriteIndices";

    transparencyPass.AddVertexBufferInput(spritePositionAttributes, BufferMemoryLayout{ { BufferDataType::Float3, "a_Position" } });
    transparencyPass.AddVertexBufferInput(spriteTexCoordAttributes, BufferMemoryLayout{ { BufferDataType::Float2, "a_TexCoord" } });
    transparencyPass.AddIndexBufferInput(spriteIndices);
    transparencyPass.AddVertexBufferInput(spriteTexIndexAttributes_Transparency, BufferMemoryLayout{ { BufferDataType::UnsignedInt, "a_TexIndex" } });
    transparencyPass.AddVertexBufferInput(spriteTransformAttribute_Transparency, BufferMemoryLayout{ { BufferDataType::Mat4, "a_Transform" } });

    TextureDescriptorBase resolveColorAttachment{};
    resolveColorAttachment.SizeX = RenderingAPI::DefaultWidth;
    resolveColorAttachment.SizeY = RenderingAPI::DefaultHeight;
    resolveColorAttachment.InternalFormat = INTERNAL_FORMAT_RGB8;
    resolveColorAttachment.Samples = 1;

    TextureDescriptorBase resolveDepthAttachment;
    resolveDepthAttachment.SizeX = RenderingAPI::DefaultWidth;
    resolveDepthAttachment.SizeY = RenderingAPI::DefaultHeight;
    resolveDepthAttachment.InternalFormat = INTERNAL_FORMAT_DEPTH_COMPONENT_32;
    resolveDepthAttachment.Samples = 1;

    std::string spritePassColorAttachment = "SpritePassColorAttachment";
    std::string spritePassDepthAttachment = "SpritePassDepthAttachment";
    std::string spritePassColorResolveOutput = "SpritePassColorResolveOutput";
    std::string spritePassDepthResolveOutput = "SpritePassDepthResolveOutput";

    device.CreateAttachment(spritePassColorResolveOutput, resolveColorAttachment, AttachmentDescriptor{ COLOR_ATTACHMENT_0 });
    device.CreateAttachment(spritePassDepthResolveOutput, resolveDepthAttachment, AttachmentDescriptor{ DEPTH_ATTACHMENT });

    transparencyPass.AddAttachmentInput(spritePassColorAttachment);
    transparencyPass.SetDepthStencilInput(spritePassDepthAttachment);
    transparencyPass.AddAttachmentOutput(spritePassColorAttachment);
    transparencyPass.SetDepthStencilOutput(spritePassDepthAttachment);
    transparencyPass.AddResolveOutput(spritePassColorResolveOutput);
    transparencyPass.AddResolveOutput(spritePassDepthResolveOutput);
    transparencyPass.AddSamplerInput("SpriteInstancedSampler");

    transparencyPass.RecordCommandBuffer([&device, &scene, &camera, &clear_color, textureMap](RenderingAPI& api, RendererID vao)
      {
        std::unordered_map<std::string, uint32_t> map = textureMap;
        UpdateSpritesInstanced_Transparent(device, api, scene, map);
        RenderSpritesInstanced(device, api, scene, vao, camera);
      }
    );

  }

  static void CreateAndUploadDefaultWhiteTexture(Device& device, uint32_t samplerWidth, uint32_t samplerHeight)
  {
    uint32_t* whiteDefaultData = new uint32_t[samplerWidth * samplerHeight];
    for (int i = 0; i < samplerWidth; i++)
      for (int j = 0; j < samplerHeight; j++)
        whiteDefaultData[i * samplerWidth + j] = 0xffffffff;

    Image whiteDefault;
    whiteDefault.DataType = UnsignedByte;
    whiteDefault.Format = IMAGE_FORMAT_RGBA;
    whiteDefault.Width = samplerWidth;
    whiteDefault.Height = samplerHeight;
    whiteDefault.Data = &whiteDefaultData[0];

    device.UploadSamplerData("SpriteInstancedSampler", whiteDefault, 0);
    delete[samplerWidth * samplerHeight] whiteDefaultData;
  }

  void AddSpritePassInstanced(Device& device, RenderGraph& renderGraph, Scene& scene, Camera& camera, std::unordered_map<std::string, uint32_t> textureMap, uint32_t maxSpriteCount, uint32_t samplerWidth, uint32_t samplerHeight, glm::vec4& clear_color)
  {

    RenderPass& spritePassInstanced = *renderGraph.AddPass("SpritePassInstanced");

    RenderPassState renderState({ Capability::DepthTest, Capability::Blend });
    renderState.DepthFunction = DepthFunction::LEqual;
    renderState.DepthMask = true;
    renderState.BlendState = BlendState{ BlendFunction::SrcAlpha, BlendFunction::OneMinusSrcAlpha };
    spritePassInstanced.SetRenderState(std::move(renderState));
    spritePassInstanced.SetShaderProgram("assets/shaders/SpriteInstanced.glsl");

    BufferDescriptor perFrameBufferDesc;
    perFrameBufferDesc.Size = 4 * 4 * 4;
    perFrameBufferDesc.Usage = STATIC_DRAW;
    device.CreateBuffer("PerFrameBuffer", perFrameBufferDesc);

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

    std::string spritePositionAttributes = "SpritePositionAttributes";
    BufferDescriptor spritePositionBufferDesc;
    spritePositionBufferDesc.Size = sizeof(vertexPositions);
    spritePositionBufferDesc.Usage = STATIC_DRAW;
    device.CreateBuffer(spritePositionAttributes, spritePositionBufferDesc, vertexPositions);

    std::string spriteTexCoordAttributes = "SpriteTexCoordAttributes";
    BufferDescriptor spriteTexCoordBufferDesc;
    spriteTexCoordBufferDesc.Size = sizeof(vertexTexCoords);
    spriteTexCoordBufferDesc.Usage = STATIC_DRAW;
    device.CreateBuffer(spriteTexCoordAttributes, spriteTexCoordBufferDesc, vertexTexCoords);

    std::string spriteTexIndexAttributes = "SpriteTexIndexAttribute";
    PersistentBufferDescriptor spriteTexIndexAttributeDesc;
    spriteTexIndexAttributeDesc.AttributeDivisor = 1;
    spriteTexIndexAttributeDesc.Size = 4 * maxSpriteCount;
    spriteTexIndexAttributeDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
    device.CreatePersistentBuffer(spriteTexIndexAttributes, spriteTexIndexAttributeDesc);

    std::string spriteTransformAttribute = "SpriteTransformAttribute";
    PersistentBufferDescriptor spriteTransformAttributeDesc;
    spriteTransformAttributeDesc.AttributeDivisor = 1;
    spriteTransformAttributeDesc.Size = 64 * maxSpriteCount;
    spriteTransformAttributeDesc.Flags = MAP_WRITE_BIT | MAP_PERSISTENT_BIT | MAP_COHERENT_BIT;
    device.CreatePersistentBuffer(spriteTransformAttribute, spriteTransformAttributeDesc);

    std::string spriteIndices = "SpriteIndices";
    BufferDescriptor spriteIndexBufferDesc;
    spriteIndexBufferDesc.Size = sizeof(indices);
    spriteIndexBufferDesc.Usage = STATIC_DRAW;
    device.CreateBuffer(spriteIndices, spriteIndexBufferDesc, indices);

    spritePassInstanced.AddVertexBufferInput(spritePositionAttributes, BufferMemoryLayout{ { BufferDataType::Float3, "a_Position" } });
    spritePassInstanced.AddVertexBufferInput(spriteTexCoordAttributes, BufferMemoryLayout{ { BufferDataType::Float2, "a_TexCoord" } });
    spritePassInstanced.AddVertexBufferInput(spriteTexIndexAttributes, BufferMemoryLayout{ { BufferDataType::UnsignedInt, "a_TexIndex" } });
    spritePassInstanced.AddVertexBufferInput(spriteTransformAttribute, BufferMemoryLayout{ { BufferDataType::Mat4, "a_Transform" } });
    spritePassInstanced.AddIndexBufferInput(spriteIndices);

    TextureDescriptorBase colorTextureDesc{};
    colorTextureDesc.SizeX = RenderingAPI::DefaultWidth;
    colorTextureDesc.SizeY = RenderingAPI::DefaultHeight;
    colorTextureDesc.InternalFormat = INTERNAL_FORMAT_RGB8;
    colorTextureDesc.Samples = 4;

    TextureDescriptorBase depthTextureDesc{};
    depthTextureDesc.SizeX = colorTextureDesc.SizeX;
    depthTextureDesc.SizeY = colorTextureDesc.SizeY;
    depthTextureDesc.InternalFormat = INTERNAL_FORMAT_DEPTH_COMPONENT_32;
    depthTextureDesc.Samples = 4;

    std::string spritePassColorAttachment = "SpritePassColorAttachment";
    std::string spritePassDepthAttachment = "SpritePassDepthAttachment";

    device.CreateAttachment(spritePassColorAttachment, colorTextureDesc, AttachmentDescriptor{ COLOR_ATTACHMENT_0 });
    device.CreateAttachment(spritePassDepthAttachment, depthTextureDesc, AttachmentDescriptor{ DEPTH_ATTACHMENT });

    spritePassInstanced.AddAttachmentOutput(spritePassColorAttachment);
    spritePassInstanced.SetDepthStencilOutput(spritePassDepthAttachment);

    SamplerDescriptor samplerDesc;
    TextureDescriptorBase samplerTextureDesc;
    samplerTextureDesc.InternalFormat = INTERNAL_FORMAT_RGBA8;
    samplerTextureDesc.Layers = 32;
    samplerTextureDesc.SizeX = samplerWidth;
    samplerTextureDesc.SizeY = samplerHeight;
    samplerTextureDesc.Type = TEXTURE_2D_ARRAY;

    device.CreateSampler("SpriteInstancedSampler", samplerTextureDesc, samplerDesc);
    spritePassInstanced.AddSamplerInput("SpriteInstancedSampler");

    //CreateAndUploadDefaultWhiteTexture(device, samplerWidth, samplerHeight);

    for (const auto& [path, index] : textureMap)
    {
      Image img = LoadImageFromFile(path);
      device.UploadSamplerData("SpriteInstancedSampler", img, index);
    }

    spritePassInstanced.RecordCommandBuffer([&device, &scene, &camera, &clear_color](RenderingAPI& api, RendererID vao)
      {
        api.SetClearColor(clear_color);
        api.SetClearDepth(1.0f);
        api.Clear(COLOR_BUFFER | DEPTH_BUFFER);

        UpdateInstancedSpriteConstants(device, api, camera);
        UpdateSpritesInstanced(device, api, scene);
        RenderSpritesInstanced(device, api, scene, vao, camera);
      }
    );

    AddTransparencySubpass(device, renderGraph, scene, camera, textureMap, maxSpriteCount, samplerWidth, samplerHeight, clear_color);

  }

}
#include "pch.h"

#include "SceneComponents.h"

namespace Pistachio
{

  StaticMesh::StaticMesh(Device& device, uint32_t indexCount, uint32_t vertexCount, BufferDescriptor indexBufferDescriptor, BufferDescriptor positionBufferDescriptor)
    :
    m_IndexCount(indexCount), m_VertexCount(vertexCount)
  {

    IndexBuffer = device.CreateBuffer(indexBufferDescriptor);
    PositionBuffer = device.CreateBuffer(positionBufferDescriptor);

  }

  void StaticMesh::SetupNormalBuffer(Device& device, BufferDescriptor bufferDescriptor)
  {
    NormalBuffer = device.CreateBuffer(bufferDescriptor);
  }

  void StaticMesh::SetupTexCoordBuffer(Device& device, int index, BufferDescriptor bufferDescriptor)
  {
    if (index == 0)
      TexCoordBuffer_0 = device.CreateBuffer(bufferDescriptor);
    else if (index == 1)
      TexCoordBuffer_1 = device.CreateBuffer(bufferDescriptor);
    else
      PSTC_ERROR("Attempted to initialize unsupported texture coordinate buffer: {0}. Only TEXCOORD_0 and TEXCOORD_1 supported for gltf files!", index);
  }

  PBRMetallicRoughnessMaterial::PBRMetallicRoughnessMaterial()
    : ColorFactor(glm::vec4(1.0f)), MetallicFactor(0.0f), RoughnessFactor(0.5f)
  {
  }

  PBRMetallicRoughnessMaterial::PBRMetallicRoughnessMaterial(glm::vec4 colorFactor, float metallicFactor, float roughnessFactor)
    : ColorFactor(colorFactor), MetallicFactor(metallicFactor), RoughnessFactor(roughnessFactor)
  {
  }

  void PBRMetallicRoughnessMaterial::SetUpColorTexture(Device& device, SamplerDescriptor& colorSamplerDescriptor, const Image& image)
  {
    ColorMap = device.CreateSampler(colorSamplerDescriptor);
    device.UploadSamplerData(ColorMap, image);
  }

  void PBRMetallicRoughnessMaterial::SetUpMetallicRoughnessMap(Device& device, SamplerDescriptor& metallicRoughnessSamplerDescriptor, const Image& image)
  {
    MetallicRoughnessMap = device.CreateSampler(metallicRoughnessSamplerDescriptor);
    device.UploadSamplerData(MetallicRoughnessMap, image);
  }

  void PBRMetallicRoughnessMaterial::SetUpNormalMap(Device& device, SamplerDescriptor& normalSamplerDescriptor, const Image& image)
  {
    NormalMap = device.CreateSampler(normalSamplerDescriptor);
    device.UploadSamplerData(NormalMap, image);
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

    BufferDescriptor modelUniformBufferDescriptor;
    modelUniformBufferDescriptor.Size = sizeof(PBRModelUniformData);
    pbrPassData.ModelUniformBuffer.DeviceBuffer = device.CreateBuffer(modelUniformBufferDescriptor);

    BufferDescriptor materialUniformBufferDescriptor;
    materialUniformBufferDescriptor.Size = sizeof(PBRMaterialUniformData);

    PBRMaterialUniformData defaultMaterial;
    defaultMaterial.colorFactor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    defaultMaterial.metallicRoughnessFactor = glm::vec2(0.0f, 0.5f);

    pbrPassData.MaterialUniformBuffer.DeviceBuffer = device.CreateBuffer(materialUniformBufferDescriptor, &defaultMaterial);

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

  void UpdateModelUniformBuffer(PBRPassData& pbrPassData, const Transform& transform)
  {
    pbrPassData.ModelUniformBuffer.Data.ModelTransform = transform;
    pbrPassData.ModelUniformBuffer.Data.NormalMatrix = glm::transpose(glm::inverse(transform));
    
    pbrPassData.ModelUniformBuffer.DeviceBuffer->Wait(); // wait for the GPU to be finished rendering last frame (not ideal, should be double or triple buffered to reduce wait)
    memcpy(pbrPassData.ModelUniformBuffer.DeviceBuffer->MemoryPtr, &pbrPassData.ModelUniformBuffer.Data, sizeof(PBRModelUniformData));
  }

  void UpdateMaterialUniformBuffer(PBRPassData& pbrPassData, PBRMetallicRoughnessMaterial material)
  {
    pbrPassData.MaterialUniformBuffer.Data.colorFactor = material.ColorFactor;
    pbrPassData.MaterialUniformBuffer.Data.metallicRoughnessFactor[0] = material.MetallicFactor;
    pbrPassData.MaterialUniformBuffer.Data.metallicRoughnessFactor[1] = material.RoughnessFactor;

    pbrPassData.MaterialUniformBuffer.DeviceBuffer->Wait(); // wait for the GPU to be finished rendering last frame (not ideal, should be double or triple buffered to reduce wait)
    memcpy(pbrPassData.MaterialUniformBuffer.DeviceBuffer->MemoryPtr, &pbrPassData.MaterialUniformBuffer.Data, sizeof(PBRMaterialUniformData));
  }

  void BeginFrame(PBRPassData& pbrPassData, const glm::vec4& clearColor, Camera& camera)
  {
    pbrPassData.RenderPass->SetRenderState(pbrPassData.RenderPassState);
    pbrPassData.RenderPass->SetShaderProgram("assets/shaders/PBR.glsl");

    pbrPassData.RenderPass->RecordCommandBuffer([&clearColor, &camera](Device& device, RenderingAPI& api)
      {
        api.SetClearColor(clearColor);
        api.SetClearDepth(1.0f);
        api.Clear(COLOR_BUFFER | DEPTH_BUFFER);
        api.BindBuffer(UNIFORM_BUFFER_TARGET, camera.GetBuffer()->RendererID, 0);
      }
    );
  }

  void Draw(const MaterialMesh& materialMesh, Camera& camera, Device& device, PBRPassData& pbrPassData)
  {
    AttributeLayoutDescriptor attributeDesc;
    attributeDesc.push_back({ materialMesh.Mesh.PositionBuffer, 0, 1, { {materialMesh.Mesh.PositionBuffer->Descriptor.DataType, "a_Position"} } });
    attributeDesc.push_back({ materialMesh.Mesh.NormalBuffer, 1, 2, { {materialMesh.Mesh.NormalBuffer->Descriptor.DataType, "a_Normal"} } });

    if (materialMesh.Mesh.TexCoordBuffer_0)
      attributeDesc.push_back({ materialMesh.Mesh.TexCoordBuffer_0, 2, 3, { {materialMesh.Mesh.TexCoordBuffer_0->Descriptor.DataType, "a_Texcoord"} } });

    AttributeLayout& attributebuteLayout = device.RequestAttributeLayout(attributeDesc, materialMesh.Mesh.IndexBuffer->RendererID);

    uint32_t count = materialMesh.Mesh.GetIndexCount();
    RendererID vao = attributebuteLayout.RendererID;
    uint32_t indexBufferBaseType = ShaderDataTypeToOpenGLBaseType(materialMesh.Mesh.IndexBuffer->Descriptor.DataType);
    int64_t colorSamplerId = materialMesh.Material.ColorMap ? materialMesh.Material.ColorMap->RendererID : -1;
    int64_t metallicRoughnessSamplerId = materialMesh.Material.MetallicRoughnessMap ? materialMesh.Material.MetallicRoughnessMap->RendererID : -1;

    pbrPassData.RenderPass->RecordCommandBuffer([&camera, vao, count, indexBufferBaseType, colorSamplerId, metallicRoughnessSamplerId, &pbrPassData](Device& device, RenderingAPI& api)
      {
        if (colorSamplerId > 0)
          api.BindSampler(colorSamplerId, 0);
        if (metallicRoughnessSamplerId > 0)
          api.BindSampler(metallicRoughnessSamplerId, 1);
        api.BindBuffer(UNIFORM_BUFFER_TARGET, pbrPassData.ModelUniformBuffer.DeviceBuffer->RendererID, 1);
        api.BindBuffer(UNIFORM_BUFFER_TARGET, pbrPassData.MaterialUniformBuffer.DeviceBuffer->RendererID, 2);
        api.DrawIndexed(vao, count, PRIMITIVE_TRIANGLES, indexBufferBaseType);

        camera.GetBuffer()->Lock();
        pbrPassData.ModelUniformBuffer.DeviceBuffer->Lock(); // lock buffer to prevent CPU from writing before the draw call is done
        pbrPassData.MaterialUniformBuffer.DeviceBuffer->Lock(); // lock buffer to prevent CPU from writing before the draw call is done
      }
    );
  }

}

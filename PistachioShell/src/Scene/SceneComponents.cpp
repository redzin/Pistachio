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

  void StaticMesh::SetupColorBuffer(Device& device, BufferDescriptor bufferDescriptor)
  {
    ColorBuffer = device.CreateBuffer(bufferDescriptor);
  }

  void StaticMesh::SetupNormalBuffer(Device& device, BufferDescriptor bufferDescriptor)
  {
    NormalBuffer = device.CreateBuffer(bufferDescriptor);
  }

  void StaticMesh::SetupTangentAndBitangentBuffers(Device& device, BufferDescriptor bufferDescriptor)
  {
    TangentBuffer = device.CreateBuffer(bufferDescriptor);
    BitangentBuffer = device.CreateBuffer(bufferDescriptor);
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
    : UniformData({ glm::vec4(1.0f), glm::vec2(0.0f, 0.5f) })
  {
  }

  void PBRMetallicRoughnessMaterial::SetupUniforms(Device& device)
  {

    BufferDescriptor materialUniformBufferDescriptor;
    materialUniformBufferDescriptor.Size = sizeof(PBRMaterialUniformData);

    DeviceUniformBuffer = device.CreateBuffer(materialUniformBufferDescriptor);

  }

  void PBRMetallicRoughnessMaterial::SetupUniforms(Device& device, PBRMaterialUniformData data)
  {

    BufferDescriptor materialUniformBufferDescriptor;
    materialUniformBufferDescriptor.Size = sizeof(PBRMaterialUniformData);

    UniformData = data;

    DeviceUniformBuffer = device.CreateBuffer(materialUniformBufferDescriptor, &UniformData);

  }

  void PBRMetallicRoughnessMaterial::UpdateUniforms()
  {
    DeviceUniformBuffer->Wait(); // wait for the GPU to be finished rendering last frame (not ideal, should be double or triple buffered to reduce wait)
    memcpy(DeviceUniformBuffer->MemoryPtr, &UniformData, sizeof(PBRMaterialUniformData));
  }

  void PBRMetallicRoughnessMaterial::UpdateUniforms(PBRMaterialUniformData data)
  {
    UniformData = data;
    DeviceUniformBuffer->Wait(); // wait for the GPU to be finished rendering last frame (not ideal, should be double or triple buffered to reduce wait)
    memcpy(DeviceUniformBuffer->MemoryPtr, &UniformData, sizeof(PBRMaterialUniformData));
  }

  void PBRMetallicRoughnessMaterial::SetupColorTexture(Device& device, SamplerDescriptor& colorSamplerDescriptor, const Image& image)
  {
    ColorMap = device.CreateSampler(colorSamplerDescriptor);
    device.UploadSamplerData(ColorMap, image);
  }

  void PBRMetallicRoughnessMaterial::SetupMetallicRoughnessMap(Device& device, SamplerDescriptor& metallicRoughnessSamplerDescriptor, const Image& image)
  {
    MetallicRoughnessMap = device.CreateSampler(metallicRoughnessSamplerDescriptor);
    device.UploadSamplerData(MetallicRoughnessMap, image);
  }

  void PBRMetallicRoughnessMaterial::SetupNormalMap(Device& device, SamplerDescriptor& normalSamplerDescriptor, const Image& image)
  {
    NormalMap = device.CreateSampler(normalSamplerDescriptor);
    device.UploadSamplerData(NormalMap, image);
  }

  Hash GetHash(PBRMetallicRoughnessMaterial material)
  {
    Hasher hasher;

    hasher.hash(material.UniformData.ColorFactor.x);
    hasher.hash(material.UniformData.ColorFactor.y);
    hasher.hash(material.UniformData.ColorFactor.z);
    hasher.hash(material.UniformData.ColorFactor.w);
    hasher.hash(material.UniformData.MetallicRoughnessFactor.x);
    hasher.hash(material.UniformData.MetallicRoughnessFactor.y);
    hasher.hash(material.MetallicRoughnessMap == nullptr);
    hasher.hash(material.NormalMap == nullptr);

    return hasher.get();
  }

  ShaderDescriptor GenerateShaderDescriptor(const PBRMetallicRoughnessMaterial& material, const StaticMesh& mesh, PBRShaderOverrides& shaderOverrides)
  {
    ShaderDescriptor shaderDescriptor;
    shaderDescriptor.Path = "assets/shaders/PBR.glsl";

    if (mesh.TexCoordBuffer_0)
      shaderDescriptor.PrependSource += "#define _ENABLE_TEXCOORD_0\n";

    if (mesh.ColorBuffer)
    {
      uint32_t componentCount = DataTypeComponentCount(mesh.ColorBuffer->Descriptor.DataType);
      if(componentCount == 3)
        shaderDescriptor.PrependSource += "#define _ENABLE_COLOR_BUFFER_3\n";
      else
        shaderDescriptor.PrependSource += "#define _ENABLE_COLOR_BUFFER_4\n";
    }

    if (material.ColorMap)
      shaderDescriptor.PrependSource += "#define _ENABLE_COLOR_TEXTURE\n";

    if (material.MetallicRoughnessMap)
      shaderDescriptor.PrependSource += "#define _ENABLE_METALLIC_ROUGHNESS_TEXTURE\n";

    if (mesh.TangentBuffer && material.NormalMap)
      shaderDescriptor.PrependSource += "#define _ENABLE_NORMAL_MAPPING\n";

    return shaderDescriptor;
  }


  PBRPassData CreatePBRPass(Device& device, RenderGraph& renderGraph)
  {
    PBRPassData pbrPassData;

    pbrPassData.RenderPassState.Capabilities = { Capability::DepthTest, Capability::Blend };
    pbrPassData.RenderPassState.DepthFunction = DepthFunction::LEqual;
    pbrPassData.RenderPassState.DepthMask = true;
    pbrPassData.RenderPassState.BlendState = BlendState{ BlendFunction::SrcAlpha, BlendFunction::OneMinusSrcAlpha };

    BufferDescriptor modelUniformBufferDescriptor;
    modelUniformBufferDescriptor.Size = 2 * sizeof(glm::mat4);
    pbrPassData.DevicePerMeshUniformBuffer = device.CreateBuffer(modelUniformBufferDescriptor);

    pbrPassData.PrePass = CreateRef<RenderPass>("PBR_Pass_PrePass");
    renderGraph.AddPass(pbrPassData.PrePass);

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

      pbrPassData.PrePass->ClearAttachmentOutputs();
      pbrPassData.PrePass->AddAttachmentOutput(pbrPassData.ColorAttachment);
      pbrPassData.PrePass->AddAttachmentOutput(pbrPassData.DepthAttachment);

      for (auto& [hash, renderPass] : pbrPassData.RenderPasses)
      {
        renderPass->ClearAttachmentOutputs();
        renderPass->AddAttachmentOutput(pbrPassData.ColorAttachment);
        renderPass->AddAttachmentOutput(pbrPassData.DepthAttachment);
      }
    }
  }

  void UpdateModelUniformBuffer(PBRPassData& pbrPassData, const Transform& transform)
  {
    pbrPassData.DevicePerMeshUniformBuffer->Wait(); // wait for the GPU to be finished rendering last frame (not ideal, should be double or triple buffered to reduce wait)

    glm::mat4* gpuPtr = (glm::mat4*)pbrPassData.DevicePerMeshUniformBuffer->MemoryPtr;
    
    gpuPtr[0] = transform;
    gpuPtr[1] = glm::transpose(glm::inverse(transform));
  }

  void BeginFrame(PBRPassData& pbrPassData, const glm::vec4& clearColor, Camera& camera)
  {
    pbrPassData.PrePass->SetRenderState(pbrPassData.RenderPassState);
    pbrPassData.PrePass->RecordCommandBuffer([&clearColor, &camera](Device& device, RenderingAPI& api)
      {
        api.SetClearColor(clearColor);
        api.SetClearDepth(1.0f);
        api.Clear(COLOR_BUFFER | DEPTH_BUFFER);
        api.BindBuffer(UNIFORM_BUFFER_TARGET, camera.GetBuffer()->RendererID, 0);
      }
    );
  }

  Ref<RenderPass> AddNewRenderPass(PBRPassData& pbrPassData, const PBRMetallicRoughnessMaterial& material, const StaticMesh& mesh, RenderGraph& renderGraph, PBRShaderOverrides& shaderOverrides)
  {
    ShaderDescriptor shaderDescriptor = GenerateShaderDescriptor(material, mesh, shaderOverrides);

    Hash materialHash = GetHash(material);
    pbrPassData.RenderPasses[materialHash] = CreateRef<RenderPass>(std::move("PBR_Pass_" + std::to_string(materialHash)));
    pbrPassData.RenderPasses[materialHash]->SetShaderProgram(shaderDescriptor);
    pbrPassData.RenderPasses[materialHash]->SetRenderState(pbrPassData.RenderPassState);
    pbrPassData.RenderPasses[materialHash]->AddAttachmentOutput(pbrPassData.ColorAttachment);
    pbrPassData.RenderPasses[materialHash]->AddAttachmentOutput(pbrPassData.DepthAttachment);

    renderGraph.AddPass(pbrPassData.RenderPasses[materialHash]);
    return pbrPassData.RenderPasses[materialHash];
  }

  void Draw(const StaticMesh& mesh, const PBRMetallicRoughnessMaterial& material, const Transform& transform, Camera& camera, Device& device, PBRPassData& pbrPassData)
  {
    AttributeLayoutDescriptor attributeDesc;
    attributeDesc.push_back({ mesh.PositionBuffer, 0, 1, { {mesh.PositionBuffer->Descriptor.DataType, "a_Position"} } });

    if (mesh.NormalBuffer)
      attributeDesc.push_back({ mesh.NormalBuffer, 1, 2, { {mesh.NormalBuffer->Descriptor.DataType, "a_Normal"} } });

    if (mesh.TexCoordBuffer_0)
      attributeDesc.push_back({ mesh.TexCoordBuffer_0, 2, 3, { {mesh.TexCoordBuffer_0->Descriptor.DataType, "a_Texcoord"} } });

    if (mesh.ColorBuffer)
      if (DataTypeComponentCount(mesh.ColorBuffer->Descriptor.DataType) == 3)
        attributeDesc.push_back({ mesh.ColorBuffer, 3, 4, { {mesh.ColorBuffer->Descriptor.DataType, "a_Color"} } });
      else
        attributeDesc.push_back({ mesh.ColorBuffer, 4, 5, { {mesh.ColorBuffer->Descriptor.DataType, "a_Color"} } });

    if (mesh.TangentBuffer && mesh.BitangentBuffer && material.NormalMap)
    {
      attributeDesc.push_back({ mesh.TangentBuffer, 5, 6, { {mesh.TangentBuffer->Descriptor.DataType, "a_Tangent"} } });
      attributeDesc.push_back({ mesh.BitangentBuffer, 6, 7, { {mesh.BitangentBuffer->Descriptor.DataType, "a_Bitangent"} } });
    }

    AttributeLayout& attributebuteLayout = device.RequestAttributeLayout(attributeDesc, mesh.IndexBuffer->RendererID);

    RendererID vao = attributebuteLayout.RendererID;

    Hash materialHash = GetHash(material);
    Ref<RenderPass> renderPass = pbrPassData.RenderPasses[materialHash];

    renderPass->RecordCommandBuffer([&camera, transform, vao, &pbrPassData, &mesh, &material](Device& device, RenderingAPI& api)
      {
        UpdateModelUniformBuffer(pbrPassData, transform);

        if (material.ColorMap)
          api.BindSampler(material.ColorMap->RendererID, 0);
        if (material.MetallicRoughnessMap)
          api.BindSampler(material.MetallicRoughnessMap->RendererID, 1);
        if (material.NormalMap)
          api.BindSampler(material.NormalMap->RendererID, 2);
        api.BindBuffer(UNIFORM_BUFFER_TARGET, pbrPassData.DevicePerMeshUniformBuffer->RendererID, 1);
        api.BindBuffer(UNIFORM_BUFFER_TARGET, material.DeviceUniformBuffer->RendererID, 2);
        api.DrawIndexed(
          vao,
          mesh.GetIndexCount(),
          mesh.DrawingMode,
          ShaderDataTypeToOpenGLBaseType(mesh.IndexBuffer->Descriptor.DataType)
        );

        camera.GetBuffer()->Lock();
        pbrPassData.DevicePerMeshUniformBuffer->Lock(); // lock buffer to prevent CPU from writing before the draw call is done
        material.DeviceUniformBuffer->Lock();
      }
    );
  }

}

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
    : UniformData({ glm::vec4(1.0f), glm::vec2(0.0f, 0.5f) })
  {
  }

  void PBRMetallicRoughnessMaterial::SetUpUniforms(Device& device)
  {

    BufferDescriptor materialUniformBufferDescriptor;
    materialUniformBufferDescriptor.Size = sizeof(PBRMaterialUniformData);

    DeviceUniformBuffer = device.CreateBuffer(materialUniformBufferDescriptor);

  }

  void PBRMetallicRoughnessMaterial::SetUpUniforms(Device& device, PBRMaterialUniformData data)
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

  ShaderDescriptor GenerateShaderDescriptor(PBRMetallicRoughnessMaterial material)
  {

    ShaderDescriptor shaderDescriptor;
    shaderDescriptor.Path = "assets/shaders/PBR.glsl";
    shaderDescriptor.PrependSource += "#define _ENABLE_TEXCOORD_0";

    return shaderDescriptor;

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
    modelUniformBufferDescriptor.Size = 2 * sizeof(glm::mat4);
    pbrPassData.DevicePerMeshUniformBuffer = device.CreateBuffer(modelUniformBufferDescriptor);

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
    pbrPassData.DevicePerMeshUniformBuffer->Wait(); // wait for the GPU to be finished rendering last frame (not ideal, should be double or triple buffered to reduce wait)

    glm::mat4* gpuPtr = (glm::mat4*)pbrPassData.DevicePerMeshUniformBuffer->MemoryPtr;
    
    gpuPtr[0] = transform;
    gpuPtr[1] = glm::transpose(glm::inverse(transform));
  }

  void BeginFrame(PBRPassData& pbrPassData, const glm::vec4& clearColor, Camera& camera)
  {
    pbrPassData.RenderPass->SetRenderState(pbrPassData.RenderPassState);
    pbrPassData.RenderPass->RecordCommandBuffer([&clearColor, &camera](Device& device, RenderingAPI& api)
      {
        api.SetClearColor(clearColor);
        api.SetClearDepth(1.0f);
        api.Clear(COLOR_BUFFER | DEPTH_BUFFER);
        api.BindBuffer(UNIFORM_BUFFER_TARGET, camera.GetBuffer()->RendererID, 0);
      }
    );
  }

  void Draw(const StaticMesh& mesh, const PBRMetallicRoughnessMaterial& material, Camera& camera, Device& device, PBRPassData& pbrPassData)
  {
    AttributeLayoutDescriptor attributeDesc;
    attributeDesc.push_back({ mesh.PositionBuffer, 0, 1, { {mesh.PositionBuffer->Descriptor.DataType, "a_Position"} } });
    
    if (mesh.NormalBuffer)
      attributeDesc.push_back({ mesh.NormalBuffer, 1, 2, { {mesh.NormalBuffer->Descriptor.DataType, "a_Normal"} } });

    if (mesh.TexCoordBuffer_0)
      attributeDesc.push_back({ mesh.TexCoordBuffer_0, 2, 3, { {mesh.TexCoordBuffer_0->Descriptor.DataType, "a_Texcoord"} } });

    AttributeLayout& attributebuteLayout = device.RequestAttributeLayout(attributeDesc, mesh.IndexBuffer->RendererID);

    uint32_t count = mesh.GetIndexCount();
    RendererID vao = attributebuteLayout.RendererID;
    uint32_t indexBufferBaseType = ShaderDataTypeToOpenGLBaseType(mesh.IndexBuffer->Descriptor.DataType);
    int64_t colorSamplerId = material.ColorMap ? material.ColorMap->RendererID : -1;
    int64_t metallicRoughnessSamplerId = material.MetallicRoughnessMap ? material.MetallicRoughnessMap->RendererID : -1;

    pbrPassData.RenderPass->RecordCommandBuffer([&camera, vao, count, indexBufferBaseType, colorSamplerId, metallicRoughnessSamplerId, &pbrPassData, &material](Device& device, RenderingAPI& api)
      {
        if (colorSamplerId > 0)
          api.BindSampler(colorSamplerId, 0);
        if (metallicRoughnessSamplerId > 0)
          api.BindSampler(metallicRoughnessSamplerId, 1);
        api.BindBuffer(UNIFORM_BUFFER_TARGET, pbrPassData.DevicePerMeshUniformBuffer->RendererID, 1);
        api.BindBuffer(UNIFORM_BUFFER_TARGET, material.DeviceUniformBuffer->RendererID, 2);
        api.DrawIndexed(vao, count, PRIMITIVE_TRIANGLES, indexBufferBaseType);

        camera.GetBuffer()->Lock();
        pbrPassData.DevicePerMeshUniformBuffer->Lock(); // lock buffer to prevent CPU from writing before the draw call is done
        material.DeviceUniformBuffer->Lock();
      }
    );
  }

}

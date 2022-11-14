#pragma once
#include "pch.h"
#include "Camera/Camera.h"

namespace Pistachio
{

  using SemanticNameComponent = std::string;

  struct TransparentRenderableComponent { };

  using Transform = glm::mat4;

  struct PrimarySceneCameraTag {};

  struct EditorCameraTag {};

  struct StaticMesh
  {

    StaticMesh(Device& device, uint32_t indexCount, uint32_t vertexCount, BufferDescriptor indexBufferDescriptor, BufferDescriptor positionBufferDescriptor);
    
    Ref<Buffer> IndexBuffer;
    Ref<Buffer> PositionBuffer;

    Ref<Buffer> ColorBuffer;
    Ref<Buffer> NormalBuffer;
    Ref<Buffer> TangentBuffer;
    Ref<Buffer> TexCoordBuffer_0;
    Ref<Buffer> TexCoordBuffer_1;

    void SetupColorBuffer(Device& device, BufferDescriptor bufferDescriptor);
    void SetupNormalBuffer(Device& device, BufferDescriptor bufferDescriptor);
    void SetupTangentBuffer(Device& device, BufferDescriptor bufferDescriptor);
    void SetupTexCoordBuffer(Device& device, int index, BufferDescriptor bufferDescriptor);

    uint32_t GetIndexCount() const { return m_IndexCount; };
    uint32_t GetVertexCount() const { return m_VertexCount; };

  private:
    uint32_t m_IndexCount;
    uint32_t m_VertexCount;

  };

  struct PBRMaterialUniformData
  {
    glm::vec4 ColorFactor; // w coordinate is unused but necessary for std140 padding
    glm::vec2 MetallicRoughnessFactor; // x is metallic factor, y is roughness factor
  };

  struct PBRMetallicRoughnessMaterial
  {
    PBRMetallicRoughnessMaterial();

    void SetupUniforms(Device& device);
    void SetupUniforms(Device& device, PBRMaterialUniformData data);
    void UpdateUniforms();
    void UpdateUniforms(PBRMaterialUniformData data);
    void SetupColorTexture(Device& device, SamplerDescriptor& colorSamplerDescriptor, const Image& image);
    void SetupMetallicRoughnessMap(Device& device, SamplerDescriptor& metallicRoughnessSamplerDescriptor, const Image& image);
    void SetupNormalMap(Device& device, SamplerDescriptor& normalSamplerDescriptor, const Image& image);

    PBRMaterialUniformData UniformData;
    Ref<Buffer> DeviceUniformBuffer;

    Ref<Sampler> ColorMap;
    Ref<Sampler> MetallicRoughnessMap;
    Ref<Sampler> NormalMap;

  };
  Hash GetHash(PBRMetallicRoughnessMaterial material);

  ShaderDescriptor GenerateShaderDescriptor(const PBRMetallicRoughnessMaterial& material, const StaticMesh& mesh);

  struct PBRPassData
  {
    Ref<RenderPass> PrePass;
    std::map<Hash, Ref<RenderPass>> RenderPasses;
    Ref<Buffer> DevicePerMeshUniformBuffer;
    
    RenderPassState RenderPassState;
    Ref<Attachment> ColorAttachment;
    Ref<Attachment> DepthAttachment;

  };
  PBRPassData CreatePBRPass(Device& device, RenderGraph& renderGraph);
  void ResizeAttachments(PBRPassData& pbrPassData, Device& device, uint32_t viewportWidth, uint32_t viewportHeight);
  void UpdateModelUniformBuffer(PBRPassData& pbrPassData, const Transform& transform);
  void BeginFrame(PBRPassData& pbrPassData, const glm::vec4& clearColor, Camera& camera);
  Ref<RenderPass> AddNewRenderPass(PBRPassData& pbrPassData, const PBRMetallicRoughnessMaterial& material, const StaticMesh& mesh, RenderGraph& renderGraph);
  
  void Draw(const StaticMesh& materialMesh, const PBRMetallicRoughnessMaterial& material, const Transform& transform, Camera& camera, Device& device, PBRPassData& pbrPassData);

  struct MaterialMesh
  {
    PBRMetallicRoughnessMaterial Material;
    StaticMesh Mesh;
  };
  
  struct Model
  {
    std::vector<MaterialMesh> Submeshes;
  };


}


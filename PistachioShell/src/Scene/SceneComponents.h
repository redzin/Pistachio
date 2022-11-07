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

    Ref<Buffer> NormalBuffer;
    Ref<Buffer> TexCoordBuffer_0;
    Ref<Buffer> TexCoordBuffer_1;

    void SetupNormalBuffer(Device& device, BufferDescriptor bufferDescriptor);
    void SetupTexCoordBuffer(Device& device, int index, BufferDescriptor bufferDescriptor);

    uint32_t GetIndexCount() const { return m_IndexCount; };
    uint32_t GetVertexCount() const { return m_VertexCount; };

  private:
    uint32_t m_IndexCount;
    uint32_t m_VertexCount;

  };

  struct PBRMetallicRoughnessMaterial
  {
    PBRMetallicRoughnessMaterial();
    PBRMetallicRoughnessMaterial(glm::vec4 colorFactor, float metallicFactor, float roughnessFactor);

    void SetUpColorTexture(Device& device, SamplerDescriptor& colorSamplerDescriptor, const Image& image);
    void SetUpMetallicRoughnessMap(Device& device, SamplerDescriptor& metallicRoughnessSamplerDescriptor, const Image& image);
    void SetUpNormalMap(Device& device, SamplerDescriptor& normalSamplerDescriptor, const Image& image);

    glm::vec4 ColorFactor;
    float MetallicFactor;
    float RoughnessFactor;

    Ref<Sampler> ColorMap;
    Ref<Sampler> MetallicRoughnessMap;
    Ref<Sampler> NormalMap;
    
  };

  struct PBRModelUniformData
  {
    glm::mat4 ModelTransform;
    glm::mat4 NormalMatrix;
  };

  struct PBRMaterialUniformData
  {
    glm::vec4 colorFactor;
    glm::vec2 metallicRoughnessFactor;
    //float metallicFactor;
    //float roughnessFactor;
  };

  struct PBRModelUniformBuffer
  {
    PBRModelUniformData Data;
    Ref<Buffer> DeviceBuffer;
  };

  struct PBRMaterialUniformBuffer
  {
    PBRMaterialUniformData Data;
    Ref<Buffer> DeviceBuffer;
  };

  struct PBRPassData
  {
    RenderPassState RenderPassState;
    Ref<RenderPass> RenderPass;
    PBRModelUniformBuffer ModelUniformBuffer;
    PBRMaterialUniformBuffer MaterialUniformBuffer;
    Ref<Attachment> ColorAttachment;
    Ref<Attachment> DepthAttachment;
  };
  PBRPassData CreatePBRPass(Device& device, RenderGraph& renderGraph);
  void ResizeAttachments(PBRPassData& pbrPassData, Device& device, uint32_t viewportWidth, uint32_t viewportHeight);
  void UpdateModelUniformBuffer(PBRPassData& pbrPassData, const Transform& transform);
  void UpdateMaterialUniformBuffer(PBRPassData& pbrPassData, PBRMetallicRoughnessMaterial material);
  void BeginFrame(PBRPassData& pbrPassData, const glm::vec4& clearColor, Camera& camera);

  struct MaterialMesh
  {
    StaticMesh Mesh;
    PBRMetallicRoughnessMaterial Material;
  };

  void Draw(const MaterialMesh& materialMesh, Camera& camera, Device& device, PBRPassData& pbrPassData);

  struct Model
  {
    std::vector<MaterialMesh> Meshes;
  };

}


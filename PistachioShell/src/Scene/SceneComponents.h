#pragma once
#include "pch.h"

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

    Ref<Buffer> PositionBuffer;
    Ref<Buffer> NormalBuffer;
    Ref<Buffer> IndexBuffer;
    Ref<Buffer> TexCoordBuffer_0;
    Ref<Buffer> TexCoordBuffer_1;

    uint32_t GetIndexCount() const { return m_IndexCount; };
    uint32_t GetVertexCount() const { return m_VertexCount; };
    void SetupNormalBuffer(Device& device, BufferDescriptor bufferDescriptor);
    void SetupTexCoordBuffer(Device& device, int index, BufferDescriptor bufferDescriptor);

  private:
    uint32_t m_IndexCount;
    uint32_t m_VertexCount;

  };

  struct Model
  {
    std::vector<StaticMesh> Meshes;
  };


}


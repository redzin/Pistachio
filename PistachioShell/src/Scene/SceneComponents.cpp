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

}

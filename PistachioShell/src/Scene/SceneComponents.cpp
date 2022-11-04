#include "pch.h"

#include "SceneComponents.h"

namespace Pistachio
{

  StaticMesh::StaticMesh(Device& device, uint32_t indexCount, uint32_t vertexCount, BufferDescriptor indexBufferDescriptor, BufferDescriptor positionBufferDescriptor, BufferDescriptor normalBufferDescriptor)
    :
    m_IndexCount(indexCount), m_VertexCount(vertexCount)
  {

    IndexBuffer = device.CreateBuffer(indexBufferDescriptor);
    PositionBuffer = device.CreateBuffer(positionBufferDescriptor);
    NormalBuffer = device.CreateBuffer(normalBufferDescriptor);

  }

  void StaticMesh::SetupTexCoordBuffer(Device& device, int index, BufferDescriptor texCoordBufferDescriptor)
  {
    if (index == 0)
      TexCoordBuffer_0 = device.CreateBuffer(texCoordBufferDescriptor);
    if (index == 1)
      TexCoordBuffer_1 = device.CreateBuffer(texCoordBufferDescriptor);
    else
      PSTC_ERROR("Attempted to initialize unsupported texture coordinate buffer: {0}. Only TEXCOORD_0 and TEXCOORD_1 supported for gltf files!", index);
  }

}

#pragma once
#include "pch.h"
#include "SceneLoader.h"

// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"

namespace Pistachio
{
  BufferDataType GetDataType(const tinygltf::Accessor& gltfAccessor)
  {
    switch (gltfAccessor.type)
    {
    case TINYGLTF_TYPE_SCALAR:
      switch (gltfAccessor.componentType)
      {
      case TINYGLTF_COMPONENT_TYPE_BYTE:
        PSTC_ERROR("Unsupported type encountered in gltf: Byte");
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        PSTC_ERROR("Unsupported type encountered in gltf: UnsignedByte");
      case TINYGLTF_COMPONENT_TYPE_DOUBLE:
        return BufferDataType::Double;
      case TINYGLTF_COMPONENT_TYPE_FLOAT:
        return BufferDataType::Float;
      case TINYGLTF_COMPONENT_TYPE_INT:
        return BufferDataType::Int;
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
        return BufferDataType::UnsignedInt;
      case TINYGLTF_COMPONENT_TYPE_SHORT:
        return BufferDataType::Short;
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
        return BufferDataType::UnsignedShort;
      }

    case TINYGLTF_TYPE_VEC2:
      switch (gltfAccessor.componentType)
      {
      case TINYGLTF_COMPONENT_TYPE_BYTE:
        PSTC_ERROR("Unsupported type encountered in gltf: Byte2");
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        return BufferDataType::UnsignedByte2;
      case TINYGLTF_COMPONENT_TYPE_DOUBLE:
        PSTC_ERROR("Unsupported type encountered in gltf: Double2");
      case TINYGLTF_COMPONENT_TYPE_FLOAT:
        return BufferDataType::Float2;
      case TINYGLTF_COMPONENT_TYPE_INT:
        return BufferDataType::Int2;
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
        PSTC_ERROR("Unsupported type encountered in gltf: UnsignedInt2");
      case TINYGLTF_COMPONENT_TYPE_SHORT:
        PSTC_ERROR("Unsupported type encountered in gltf: Short2");
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
        return BufferDataType::UnsignedShort2;
      }

    case TINYGLTF_TYPE_VEC3:
      switch (gltfAccessor.componentType)
      {
      case TINYGLTF_COMPONENT_TYPE_BYTE:
        PSTC_ERROR("Unsupported type encountered in gltf: Byte3");
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        PSTC_ERROR("Unsupported type encountered in gltf: UnsignedByte3");
      case TINYGLTF_COMPONENT_TYPE_DOUBLE:
        PSTC_ERROR("Unsupported type encountered in gltf: Double3");
      case TINYGLTF_COMPONENT_TYPE_FLOAT:
        return BufferDataType::Float3;
      case TINYGLTF_COMPONENT_TYPE_INT:
        return BufferDataType::Int3;
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
        PSTC_ERROR("Unsupported type encountered in gltf: UnsignedInt3");
      case TINYGLTF_COMPONENT_TYPE_SHORT:
        PSTC_ERROR("Unsupported type encountered in gltf: Short3");
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
        PSTC_ERROR("Unsupported type encountered in gltf: UnsignedShort3");
      }

    case TINYGLTF_TYPE_VEC4:
      switch (gltfAccessor.componentType)
      {
      case TINYGLTF_COMPONENT_TYPE_BYTE:
        PSTC_ERROR("Unsupported type encountered in gltf: Byte4");
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        PSTC_ERROR("Unsupported type encountered in gltf: UnsignedByte4");
      case TINYGLTF_COMPONENT_TYPE_DOUBLE:
        PSTC_ERROR("Unsupported type encountered in gltf: Double4");
      case TINYGLTF_COMPONENT_TYPE_FLOAT:
        return BufferDataType::Float4;
      case TINYGLTF_COMPONENT_TYPE_INT:
        return BufferDataType::Int4;
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
        PSTC_ERROR("Unsupported type encountered in gltf: UnsignedInt4");
      case TINYGLTF_COMPONENT_TYPE_SHORT:
        PSTC_ERROR("Unsupported type encountered in gltf: Short4");
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
        PSTC_ERROR("Unsupported type encountered in gltf: UnsignedShort4");
      }

    case TINYGLTF_TYPE_MAT2:
      PSTC_ERROR("Unsupported type encountered in gltf: Mat2");

    case TINYGLTF_TYPE_MAT3:
      if (gltfAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
        return BufferDataType::Mat3;
      else
        PSTC_ERROR("Unsupported type encountered in gltf: Mat3 of non-float component type");

    case TINYGLTF_TYPE_MAT4:
      if (gltfAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT)
        return BufferDataType::Mat4;
      else
        PSTC_ERROR("Unsupported type encountered in gltf: Mat4 of non-float component type");

    default:
      return BufferDataType::Float;

    }
  }

  uint32_t ComputeBufferSize(const tinygltf::Accessor& gltfAccessor)
  {
    int coordinateCount = tinygltf::GetNumComponentsInType(gltfAccessor.type);
    int bytesPerCoordinate = tinygltf::GetComponentSizeInBytes(gltfAccessor.componentType);
    return gltfAccessor.count * coordinateCount * bytesPerCoordinate;
  }

  // Deinterleaves buffers and copies onto GPU virtual memory
  void ProcessBuffer(char* bufferDestPtr, const tinygltf::Accessor& gltfAccessor, const tinygltf::BufferView& gltfBufferView, const tinygltf::Buffer& gltfBuffer)
  {
    uint32_t offset = gltfBufferView.byteOffset + gltfAccessor.byteOffset;
    uint32_t sourceIndex = 0, destIndex = 0;
    int byteStride = gltfAccessor.ByteStride(gltfBufferView);
    int coordinateCount = tinygltf::GetNumComponentsInType(gltfAccessor.type);
    int bytesPerCoordinate = tinygltf::GetComponentSizeInBytes(gltfAccessor.componentType);

    for (int i = 0; i < gltfAccessor.count; i++)
    {
      for (int j = 0; j < coordinateCount; j++)
      {
        for (int k = 0; k < bytesPerCoordinate; k++)
        {
          // offset + element index * stride in total bytes + component index * byte size of each component + byte index through component
          sourceIndex = offset + i * byteStride + j * bytesPerCoordinate + k;
          bufferDestPtr[destIndex] = gltfBuffer.data[sourceIndex];
          destIndex++;
        }
      }
    }

  }

  Transform GetTransform(const tinygltf::Node& gltfNode)
  {
    bool hasTransform = gltfNode.matrix.size() > 0 || gltfNode.rotation.size() > 0 || gltfNode.translation.size() > 0 || gltfNode.scale.size() > 0;

    glm::mat4 transform = glm::mat4(1.0f);
    if (hasTransform)
    {

      if (gltfNode.matrix.size() > 0)
      {
        for (int i = 0; i < 4; i++)
          for (int j = 0; j < 4; j++)
            transform[i][j] = static_cast<float>(gltfNode.matrix[4 * i + j]);
      }
      else
      {
        if (gltfNode.scale.size() > 0)
          transform = glm::scale(transform, glm::vec3(gltfNode.scale[0], gltfNode.scale[1], gltfNode.scale[2]));
        
        if (gltfNode.rotation.size() > 0)
        {
          glm::quat rotQuaternion(gltfNode.rotation[0], gltfNode.rotation[1], gltfNode.rotation[2], gltfNode.rotation[3]);
          transform = glm::toMat4(rotQuaternion) * transform;
        }

        if (gltfNode.translation.size() > 0)
        {
          glm::vec3 translation = glm::vec3(gltfNode.translation[0], gltfNode.translation[1], gltfNode.translation[2]);
          transform = glm::translate(transform, translation);
        }
      }
    }
    return transform;
  }

  glm::vec4 ComputeTangent(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2)
  {
    // Edges of the triangle : position delta
    glm::vec3 deltaPos1 = p1 - p0;
    glm::vec3 deltaPos2 = p2 - p0;

    // UV delta
    glm::vec2 deltaUV1 = uv1 - uv0;
    glm::vec2 deltaUV2 = uv2 - uv0;

    // Compute tangent
    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    return glm::vec4((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * f, 1.0f);
  }

  void ComputeTangentBuffer(Device& device, StaticMesh& mesh)
  {
    uint32_t indexCount = mesh.GetIndexCount();
    uint32_t vertexCount = mesh.GetVertexCount();

    BufferDescriptor tangentBufferDescriptor;
    tangentBufferDescriptor.Size = vertexCount * sizeof(glm::vec4);
    tangentBufferDescriptor.DataType = BufferDataType::Float4;
    mesh.SetupTangentBuffer(device, tangentBufferDescriptor);

    glm::vec4* tangentBufferPtr = (glm::vec4*)mesh.TangentBuffer->MemoryPtr;

    glm::vec3 position;
    glm::vec2 texcoord;

    uint32_t index0;
    uint32_t index1;
    uint32_t index2;
    glm::vec3 deltaPos1;
    glm::vec3 deltaPos2;
    glm::vec2 deltaUV1;
    glm::vec2 deltaUV2;
    for (int i = 0; i < indexCount - 2; i += 3)
    {

      // Get the indices from the index buffer
      if (mesh.IndexBuffer->Descriptor.DataType == BufferDataType::UnsignedByte)
      {
        index0 = ((uint8_t*)mesh.IndexBuffer->MemoryPtr)[i];
        index1 = ((uint8_t*)mesh.IndexBuffer->MemoryPtr)[i + 1];
        index2 = ((uint8_t*)mesh.IndexBuffer->MemoryPtr)[i + 2];
      }
      else if (mesh.IndexBuffer->Descriptor.DataType == BufferDataType::UnsignedShort)
      {
        index0 = ((uint16_t*)mesh.IndexBuffer->MemoryPtr)[i];
        index1 = ((uint16_t*)mesh.IndexBuffer->MemoryPtr)[i + 1];
        index2 = ((uint16_t*)mesh.IndexBuffer->MemoryPtr)[i + 2];
      }
      else if (mesh.IndexBuffer->Descriptor.DataType == BufferDataType::UnsignedInt)
      {
        index0 = ((uint32_t*)mesh.IndexBuffer->MemoryPtr)[i];
        index1 = ((uint32_t*)mesh.IndexBuffer->MemoryPtr)[i + 1];
        index2 = ((uint32_t*)mesh.IndexBuffer->MemoryPtr)[i + 2];
      }
      else
        PSTC_ASSERT(false, "Invalid index buffer data type!");


      // Read the position buffer at the right indices
      glm::vec3& v0 = ((glm::vec3*)mesh.PositionBuffer->MemoryPtr)[index0]; // positions[i + 0];
      glm::vec3& v1 = ((glm::vec3*)mesh.PositionBuffer->MemoryPtr)[index1];
      glm::vec3& v2 = ((glm::vec3*)mesh.PositionBuffer->MemoryPtr)[index2];

      // Read texcoords at the right indices
      glm::vec2& uv0 = ((glm::vec2*)mesh.TexCoordBuffer_0->MemoryPtr)[index0];
      glm::vec2& uv1 = ((glm::vec2*)mesh.TexCoordBuffer_0->MemoryPtr)[index1];
      glm::vec2& uv2 = ((glm::vec2*)mesh.TexCoordBuffer_0->MemoryPtr)[index2];

      tangentBufferPtr[index0] = ComputeTangent(v0, v1, v2, uv0, uv1, uv2);
      tangentBufferPtr[index1] = ComputeTangent(v1, v2, v0, uv1, uv2, uv0);
      tangentBufferPtr[index2] = ComputeTangent(v2, v0, v1, uv2, uv0, uv1);
    }

  }

  PBRMetallicRoughnessMaterial GetMaterial(Device& device, const tinygltf::Model& gltfObject, const tinygltf::Primitive& gltfPrimitive)
  {
    PBRMetallicRoughnessMaterial material;
    material.SetupUniforms(device, { glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0, 1.0) });// gltf standard default: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.pdf
    if (gltfPrimitive.material >= 0)
    {
      const auto& gltfMaterial = gltfObject.materials[gltfPrimitive.material];

      for (int i = 0; i < 4; i++)
        material.UniformData.ColorFactor[i] = static_cast<float>(gltfMaterial.pbrMetallicRoughness.baseColorFactor[i]);

      material.UniformData.MetallicRoughnessFactor.x = static_cast<float>(gltfMaterial.pbrMetallicRoughness.metallicFactor);
      material.UniformData.MetallicRoughnessFactor.y = static_cast<float>(gltfMaterial.pbrMetallicRoughness.roughnessFactor);
      material.UpdateUniforms();

      if (gltfMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
      {
        //gltfMaterial.pbrMetallicRoughness.baseColorTexture.texCoord; // figure out how to handle this
        const auto& gltfColorTexture = gltfObject.textures[gltfMaterial.pbrMetallicRoughness.baseColorTexture.index];
        const auto& gltfColorSampler = gltfObject.samplers[gltfColorTexture.sampler];
        const auto& gltfColorImage = gltfObject.images[gltfColorTexture.source];
        SamplerDescriptor colorSamplerDescriptor;
        colorSamplerDescriptor.MagFilter = gltfColorSampler.magFilter;
        colorSamplerDescriptor.MinFilter = gltfColorSampler.minFilter;
        colorSamplerDescriptor.TextureDescriptor.InternalFormat = INTERNAL_FORMAT_RGB8;
        colorSamplerDescriptor.TextureDescriptor.SizeX = gltfColorImage.width;
        colorSamplerDescriptor.TextureDescriptor.SizeY = gltfColorImage.height;
        colorSamplerDescriptor.MipLevels = 4;
        Image colorImage;
        colorImage.Data = (void*)&gltfColorImage.image[0];
        colorImage.Width = gltfColorImage.width;
        colorImage.Height = gltfColorImage.height;
        colorImage.DataType = gltfColorImage.pixel_type;
        colorImage.Format = gltfColorImage.component == 4 ? IMAGE_FORMAT_RGBA : IMAGE_FORMAT_RGB;
        material.SetupColorTexture(device, colorSamplerDescriptor, colorImage);
      }

      if (gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0)
      {
        //gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.texCoord; // figure out how to handle this
        const auto& gltfMetallicRoughnessTexture = gltfObject.textures[gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index];
        const auto& gltfMetallicRoughnessSampler = gltfObject.samplers[gltfMetallicRoughnessTexture.sampler];
        const auto& gltfMetallicRoughnessImage = gltfObject.images[gltfMetallicRoughnessTexture.source];
        SamplerDescriptor metallicRoughnessSamplerDescriptor;
        metallicRoughnessSamplerDescriptor.MagFilter = gltfMetallicRoughnessSampler.magFilter;
        metallicRoughnessSamplerDescriptor.MinFilter = gltfMetallicRoughnessSampler.minFilter;
        metallicRoughnessSamplerDescriptor.TextureDescriptor.InternalFormat = INTERNAL_FORMAT_RGB8;
        metallicRoughnessSamplerDescriptor.TextureDescriptor.SizeX = gltfMetallicRoughnessImage.width;
        metallicRoughnessSamplerDescriptor.TextureDescriptor.SizeY = gltfMetallicRoughnessImage.height;
        metallicRoughnessSamplerDescriptor.MipLevels = 4;
        Image metallicRoughnessImage;
        metallicRoughnessImage.Data = (void*)&gltfMetallicRoughnessImage.image[0];
        metallicRoughnessImage.Width = gltfMetallicRoughnessImage.width;
        metallicRoughnessImage.Height = gltfMetallicRoughnessImage.height;
        metallicRoughnessImage.DataType = gltfMetallicRoughnessImage.pixel_type;
        metallicRoughnessImage.Format = gltfMetallicRoughnessImage.component == 4 ? IMAGE_FORMAT_RGBA : IMAGE_FORMAT_RGB;
        material.SetupMetallicRoughnessMap(device, metallicRoughnessSamplerDescriptor, metallicRoughnessImage);
      }

      if (gltfMaterial.normalTexture.index >= 0)
      {
        //gltfMaterial.normalTexture.texCoord; // figure out how to handle this
        const auto& gltfNormalTexture = gltfObject.textures[gltfMaterial.normalTexture.index];
        const auto& gltfNormalSampler = gltfObject.samplers[gltfNormalTexture.sampler];
        const auto& gltfNormalImage = gltfObject.images[gltfNormalTexture.source];
        SamplerDescriptor normalSamplerDescriptor;
        normalSamplerDescriptor.MagFilter = gltfNormalSampler.magFilter;
        normalSamplerDescriptor.MinFilter = gltfNormalSampler.minFilter;
        normalSamplerDescriptor.TextureDescriptor.InternalFormat = INTERNAL_FORMAT_RGB8;
        normalSamplerDescriptor.TextureDescriptor.SizeX = gltfNormalImage.width;
        normalSamplerDescriptor.TextureDescriptor.SizeY = gltfNormalImage.height;
        normalSamplerDescriptor.MipLevels = 1;
        Image normalImage;
        normalImage.Data = (void*)&gltfNormalImage.image[0];
        normalImage.Width = gltfNormalImage.width;
        normalImage.Height = gltfNormalImage.height;
        normalImage.DataType = gltfNormalImage.pixel_type;
        normalImage.Format = gltfNormalImage.component == 4 ? IMAGE_FORMAT_RGBA : IMAGE_FORMAT_RGB;
        material.SetupNormalMap(device, normalSamplerDescriptor, normalImage);
      }

    }
    return material;
  }

  DrawingMode GetDrawingMode(tinygltf::Primitive gltfPrimitive)
  {
    return gltfPrimitive.mode;
  }

  void ProcessNode(Device& device, Scene& scene, const tinygltf::Model& gltfObject, const tinygltf::Node& gltfNode, SceneEntity parentEntity)
  {

    SceneEntity sceneEntity = scene.CreateEntity(parentEntity);
    Transform& transform = sceneEntity.AddComponent<Transform>(GetTransform(gltfNode));
    SemanticNameComponent& name = sceneEntity.AddComponent<SemanticNameComponent>(gltfNode.name);

    if (gltfNode.mesh >= 0)
    {
      Model& model = sceneEntity.AddComponent<Model>();

      const auto& gltfMesh = gltfObject.meshes[gltfNode.mesh];

      for (const auto& gltfPrimitive : gltfMesh.primitives)
      {
        int positionAccessorIndex = -1;
        if (gltfPrimitive.attributes.count("POSITION") > 0)
          positionAccessorIndex = gltfPrimitive.attributes.at("POSITION");
        else
          PSTC_ASSERT(positionAccessorIndex >= 0, "Could not locate mesh position buffer!");

        const auto& gltfIndexAccessor = gltfObject.accessors[gltfPrimitive.indices];
        const auto& gltfIndexBufferView = gltfObject.bufferViews[gltfIndexAccessor.bufferView];
        const auto& gltfIndexBuffer = gltfObject.buffers[gltfIndexBufferView.buffer];

        const auto& gltfPositionAccessor = gltfObject.accessors[positionAccessorIndex];
        const auto& gltfPositionBufferView = gltfObject.bufferViews[gltfPositionAccessor.bufferView];
        const auto& gltfPositionBuffer = gltfObject.buffers[gltfPositionBufferView.buffer];

        BufferDescriptor indexBufferDescriptor;
        indexBufferDescriptor.Size = ComputeBufferSize(gltfIndexAccessor);
        indexBufferDescriptor.DataType = GetDataType(gltfIndexAccessor);

        BufferDescriptor positionBufferDescriptor;
        positionBufferDescriptor.Size = ComputeBufferSize(gltfPositionAccessor);
        positionBufferDescriptor.DataType = BufferDataType::Float3;

        model.Submeshes.push_back({ GetMaterial(device, gltfObject, gltfPrimitive), StaticMesh(device, gltfIndexAccessor.count, gltfPositionAccessor.count, indexBufferDescriptor, positionBufferDescriptor)});

        MaterialMesh& materialMesh = model.Submeshes[model.Submeshes.size() - 1];
        PBRMetallicRoughnessMaterial& material = materialMesh.Material;
        StaticMesh& mesh = materialMesh.Mesh;
        mesh.DrawingMode = GetDrawingMode(gltfPrimitive);

        ProcessBuffer((char*)mesh.IndexBuffer->MemoryPtr, gltfIndexAccessor, gltfIndexBufferView, gltfIndexBuffer);
        ProcessBuffer((char*)mesh.PositionBuffer->MemoryPtr, gltfPositionAccessor, gltfPositionBufferView, gltfPositionBuffer);

        if (gltfPrimitive.attributes.count("COLOR_0") > 0)
        {
          int colorAccessorIndex = gltfPrimitive.attributes.at("COLOR_0");

          const auto& gltfColorAccessor = gltfObject.accessors[colorAccessorIndex];
          const auto& gltfcolorBufferView = gltfObject.bufferViews[gltfColorAccessor.bufferView];
          const auto& gltfcolorBuffer = gltfObject.buffers[gltfcolorBufferView.buffer];

          BufferDescriptor colorBufferDescriptor;
          colorBufferDescriptor.Size = ComputeBufferSize(gltfColorAccessor);
          colorBufferDescriptor.DataType = GetDataType(gltfColorAccessor);
          mesh.SetupColorBuffer(device, colorBufferDescriptor);
          ProcessBuffer((char*)mesh.ColorBuffer->MemoryPtr, gltfColorAccessor, gltfcolorBufferView, gltfcolorBuffer);

        }
        else
        {
          PSTC_WARN("Could not locate color buffer!");
        }

        if (gltfPrimitive.attributes.count("NORMAL") > 0)
        {
          int normalAccessorIndex = gltfPrimitive.attributes.at("NORMAL");

          const auto& gltfNormalAccessor = gltfObject.accessors[normalAccessorIndex];
          const auto& gltfNormalBufferView = gltfObject.bufferViews[gltfNormalAccessor.bufferView];
          const auto& gltfNormalBuffer = gltfObject.buffers[gltfNormalBufferView.buffer];

          BufferDescriptor normalBufferDescriptor;
          normalBufferDescriptor.Size = ComputeBufferSize(gltfNormalAccessor);
          normalBufferDescriptor.DataType = BufferDataType::Float3;
          mesh.SetupNormalBuffer(device, normalBufferDescriptor);
          ProcessBuffer((char*)mesh.NormalBuffer->MemoryPtr, gltfNormalAccessor, gltfNormalBufferView, gltfNormalBuffer);

        }
        else
        {
          PSTC_WARN("Could not locate mesh normal buffer!");
        }

        if (gltfPrimitive.attributes.count("TEXCOORD_0") > 0)
        {
          int accessorIndex = gltfPrimitive.attributes.at("TEXCOORD_0");

          const auto& gltfTexcoordAccessor = gltfObject.accessors[accessorIndex];
          const auto& gltfTexcoordBufferView = gltfObject.bufferViews[gltfTexcoordAccessor.bufferView];
          const auto& gltfTexcoordBuffer = gltfObject.buffers[gltfTexcoordBufferView.buffer];

          BufferDescriptor texcoordBufferDescriptor;
          texcoordBufferDescriptor.Size = ComputeBufferSize(gltfTexcoordAccessor);
          texcoordBufferDescriptor.DataType =
            gltfTexcoordAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE ? BufferDataType::UnsignedByte2 :
            gltfTexcoordAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT ? BufferDataType::UnsignedShort2 :
            BufferDataType::Float2;
          mesh.SetupTexCoordBuffer(device, 0, texcoordBufferDescriptor);
          ProcessBuffer((char*)mesh.TexCoordBuffer_0->MemoryPtr, gltfTexcoordAccessor, gltfTexcoordBufferView, gltfTexcoordBuffer);
        }
        else
        {
          PSTC_WARN("Could not locate mesh texture coordinate buffer 0!");
        }

        if (gltfPrimitive.attributes.count("TEXCOORD_1") > 0)
        {
          int accessorIndex = gltfPrimitive.attributes.at("TEXCOORD_1");

          const auto& gltfTexcoordAccessor = gltfObject.accessors[accessorIndex];
          const auto& gltfTexcoordBufferView = gltfObject.bufferViews[gltfTexcoordAccessor.bufferView];
          const auto& gltfTexcoordBuffer = gltfObject.buffers[gltfTexcoordBufferView.buffer];

          BufferDescriptor texcoordBufferDescriptor;
          texcoordBufferDescriptor.Size = ComputeBufferSize(gltfTexcoordAccessor);
          texcoordBufferDescriptor.DataType = BufferDataType::Float2;
          mesh.SetupTexCoordBuffer(device, 1, texcoordBufferDescriptor);
          ProcessBuffer((char*)mesh.TexCoordBuffer_1->MemoryPtr, gltfTexcoordAccessor, gltfTexcoordBufferView, gltfTexcoordBuffer);
        }
        else
        {
          PSTC_WARN("Could not locate mesh texture coordinate buffer 1!");
        }


        if (gltfPrimitive.attributes.count("TANGENT") > 0 && material.NormalMap)
        {
          ComputeTangentBuffer(device, mesh);
          //int tangentAccessor = gltfPrimitive.attributes.at("TANGENT");
          //
          //const auto& gltfTangentAccessor = gltfObject.accessors[tangentAccessor];
          //const auto& gltfTangentBufferView = gltfObject.bufferViews[gltfTangentAccessor.bufferView];
          //const auto& gltfTangentBuffer = gltfObject.buffers[gltfTangentBufferView.buffer];
          //
          //BufferDescriptor tangentBufferDescriptor;
          //tangentBufferDescriptor.Size = ComputeBufferSize(gltfTangentAccessor);
          //tangentBufferDescriptor.DataType = BufferDataType::Float4;
          //mesh.SetupTangentBuffer(device, tangentBufferDescriptor);
          //ProcessBuffer((char*)mesh.TangentBuffer->MemoryPtr, gltfTangentAccessor, gltfTangentBufferView, gltfTangentBuffer);

        }
        else if (material.NormalMap)
        {
          ComputeTangentBuffer(device, mesh);
        }
        else
        {
          PSTC_WARN("Could not locate mesh tangent buffer!");
        }

      }
    }
    else if (gltfNode.camera >= 0)
    {

      const auto& gltfCamera = gltfObject.cameras[gltfNode.camera];

      if (gltfCamera.perspective.yfov != 0 && gltfCamera.perspective.aspectRatio != 0 && gltfCamera.perspective.znear != gltfCamera.perspective.zfar)
      {
        sceneEntity.AddComponent<CameraOrbitController>();
        name = gltfCamera.name != "" ? gltfCamera.name : name != ""  ? name : "Untitled Camera";
        CameraOrbitController& camOrbitController = sceneEntity.GetComponent<CameraOrbitController>();
        sceneEntity.AddComponent<Camera>(
          camOrbitController.CreatePerspectiveCamera(
            device,
            glm::vec3(1.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            5.0f,
            gltfCamera.perspective.yfov,
            1,
            1,
            gltfCamera.perspective.znear,
            gltfCamera.perspective.zfar
          )
        );
      }

    }

    for (const auto& childIndex : gltfNode.children)
    {
      ProcessNode(device, scene, gltfObject, gltfObject.nodes[childIndex], sceneEntity);
    }

  }

  std::vector<Scene> SceneLoader::LoadGLTF(Device& device, const std::string& path)
  {

    tinygltf::Model gltfObject;
    tinygltf::TinyGLTF gltfLoader;
    std::string err;
    std::string warn;

    bool ret = gltfLoader.LoadASCIIFromFile(&gltfObject, &err, &warn, path);
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

    if (!warn.empty()) {
      PSTC_WARN("tinygltf warning: {0}", warn);
    }

    if (!err.empty()) {
      PSTC_WARN("tinygltf error: {0}", err);
    }

    if (!ret) {
      PSTC_ERROR("tinygltf failed to parse glTF!");
    }

    std::vector<Scene> scenes;

    for (const auto& gltfScene : gltfObject.scenes)
    {
      scenes.push_back(Scene(gltfScene.name.empty() ? "Untitled Scene" : gltfScene.name));
      Scene& scene = scenes[scenes.size() - 1];

      for (const auto& gltfNodeIndex : gltfScene.nodes)
      {
        auto& gltfNode = gltfObject.nodes[gltfNodeIndex];

        ProcessNode(device, scene, gltfObject, gltfNode, SceneEntity(scene));
      }

    }

    return scenes;
  }
}
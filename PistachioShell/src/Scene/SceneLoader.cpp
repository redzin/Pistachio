#pragma once
#include "pch.h"
#include "SceneLoader.h"
#include "glm/gtx/quaternion.hpp"

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
        PSTC_ERROR("Unsupported type encountered in gltf: UnsignedByte2");
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
        PSTC_ERROR("Unsupported type encountered in gltf: UnsignedShort2");
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

  void ProcessNode(Device& device, Scene& scene, const tinygltf::Model& gltfObject, const tinygltf::Node& gltfNode, SceneEntity parentEntity)
  {
    SceneEntity sceneEntity = scene.CreateEntity(parentEntity);
    Model& model = sceneEntity.AddComponent<Model>();

    bool hasTransform = gltfNode.matrix.size() > 0 || gltfNode.rotation.size() > 0 || gltfNode.translation.size() > 0 || gltfNode.scale.size() > 0;

    glm::mat4 transform(1.0f);
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
        glm::scale(transform, glm::vec3(gltfNode.scale[0], gltfNode.scale[1], gltfNode.scale[2]));

        glm::quat rotQuaternion(gltfNode.rotation[0], gltfNode.rotation[1], gltfNode.rotation[2], gltfNode.rotation[3]);
        transform = glm::toMat4(rotQuaternion) * transform;

        glm::translate(transform, glm::vec3(gltfNode.translation[0], gltfNode.translation[1], gltfNode.translation[2]));

      }
    }
    sceneEntity.AddComponent<Transform>(transform);

    if (gltfNode.mesh >= 0)
    {
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

        model.Meshes.push_back(StaticMesh(device, gltfIndexAccessor.count, gltfPositionAccessor.count, indexBufferDescriptor, positionBufferDescriptor));
        StaticMesh& mesh = model.Meshes[model.Meshes.size() - 1];

        ProcessBuffer((char*)mesh.IndexBuffer->MemoryPtr, gltfIndexAccessor, gltfIndexBufferView, gltfIndexBuffer);
        ProcessBuffer((char*)mesh.PositionBuffer->MemoryPtr, gltfPositionAccessor, gltfPositionBufferView, gltfPositionBuffer);

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
          texcoordBufferDescriptor.DataType = BufferDataType::Float2;
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

      }
    }
    else if (gltfNode.camera >= 0)
    {

      const auto& gltfCamera = gltfObject.cameras[gltfNode.camera];

      if (gltfCamera.perspective.yfov != 0 && gltfCamera.perspective.aspectRatio != 0 && gltfCamera.perspective.znear != gltfCamera.perspective.zfar)
      {
        sceneEntity.AddComponent<CameraOrbitController>();
        sceneEntity.AddComponent<SemanticNameComponent>(gltfCamera.name == "" ? "Untitled camera" : gltfCamera.name);
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
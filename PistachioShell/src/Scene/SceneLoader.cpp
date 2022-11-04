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

        int normalAccessorIndex = -1;
        if (gltfPrimitive.attributes.count("NORMAL") > 0)
          normalAccessorIndex = gltfPrimitive.attributes.at("NORMAL");
        else
          PSTC_WARN("Could not locate mesh normal buffer!");

        const auto& gltfIndexAccessor = gltfObject.accessors[gltfPrimitive.indices];
        const auto& gltfIndexBufferView = gltfObject.bufferViews[gltfIndexAccessor.bufferView];
        const auto& gltfIndexBuffer = gltfObject.buffers[gltfIndexBufferView.buffer];

        const auto& gltfPositionAccessor = gltfObject.accessors[positionAccessorIndex];
        const auto& gltfPositionBufferView = gltfObject.bufferViews[gltfPositionAccessor.bufferView];
        const auto& gltfPositionBuffer = gltfObject.buffers[gltfPositionBufferView.buffer];

        const auto& gltfNormalAccessor = gltfObject.accessors[normalAccessorIndex];
        const auto& gltfNormalBufferView = gltfObject.bufferViews[gltfNormalAccessor.bufferView];
        const auto& gltfNormalBuffer = gltfObject.buffers[gltfNormalBufferView.buffer];

        BufferDescriptor indexBufferDescriptor;
        indexBufferDescriptor.Size = ComputeBufferSize(gltfIndexAccessor);
        indexBufferDescriptor.DataType = GetDataType(gltfIndexAccessor);

        BufferDescriptor positionBufferDescriptor;
        positionBufferDescriptor.Size = ComputeBufferSize(gltfPositionAccessor);
        positionBufferDescriptor.DataType = BufferDataType::Float3;

        BufferDescriptor normalBufferDescriptor;
        normalBufferDescriptor.Size = ComputeBufferSize(gltfNormalAccessor);
        normalBufferDescriptor.DataType = BufferDataType::Float3;

        model.Meshes.push_back(StaticMesh(device, gltfIndexAccessor.count, gltfPositionAccessor.count, indexBufferDescriptor, positionBufferDescriptor, normalBufferDescriptor));
        StaticMesh& mesh = model.Meshes[model.Meshes.size() - 1];

        // Deinterleaves buffers and copies onto GPU virtual memory
        ProcessBuffer((char*)mesh.IndexBuffer->MemoryPtr, gltfIndexAccessor, gltfIndexBufferView, gltfIndexBuffer);
        ProcessBuffer((char*)mesh.PositionBuffer->MemoryPtr, gltfPositionAccessor, gltfPositionBufferView, gltfPositionBuffer);
        ProcessBuffer((char*)mesh.NormalBuffer->MemoryPtr, gltfNormalAccessor, gltfNormalBufferView, gltfNormalBuffer);

      }
    }
    else if (gltfNode.camera >= 0)
    {
      // todo: add camera component to entity here
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

  //Scene LoadScene_Deprecated(const std::string& path)
  //{
  //  Scene scene;
  //
  //  std::string f = ReadFile(path);
  //  
  //  json gltf = json::parse(f);
  //  
  //  for (const auto& scene : gltf.at("scenes"))
  //  {
  //  
  //    for (const uint32_t& gltfNodeIndex : scene.at("nodes"))
  //    {
  //      ProcessNode_Deprecated(gltf, gltfNodeIndex);
  //    }
  //  
  //  }
  //
  //  return scene;
  //
  //}

  //void ProcessNode_Deprecated(const json& gltf, const uint32_t& gltfNodeIndex, const int32_t& parent = -1)
  //{
  //  auto gltfNode = gltf.at("nodes")[gltfNodeIndex];
  //  if (gltfNode.count("name") > 0)
  //    PSTC_CORE_INFO("SceneNode: {0}", gltfNode.at("name"));
  //  else
  //    PSTC_CORE_INFO("SceneNode: {0}", gltfNodeIndex);
  //
  //  glm::mat4 transform;
  //  if (gltfNode.count("matrix") > 0)
  //  {
  //    transform = gltfNode.at("matrix").get<glm::mat4>();
  //    PSTC_CORE_INFO("  Matrix loaded");
  //  }
  //
  //  if (gltfNode.count("scale") > 0 && gltfNode.count("rotation") > 0 && gltfNode.count("translation") > 0)
  //  {
  //    // Scale
  //    glm::scale(transform, gltfNode.at("scale").get<glm::vec3>());
  //
  //    // Rotate
  //    glm::quat quatRot = gltfNode.at("rotation").get<glm::quat>();
  //    glm::mat4 R = glm::toMat4(quatRot);
  //    transform = R * transform;
  //
  //    // Translate
  //    glm::translate(transform, gltfNode.at("translation").get<glm::vec3>());
  //    PSTC_CORE_INFO("  Matrix loaded");
  //  }
  //
  //  if (gltfNode.count("mesh") > 0 && gltf.count("meshes") > 0)
  //  {
  //
  //    uint32_t meshIndex = gltfNode.at("mesh").get<uint32_t>();
  //    const auto& gltfMesh = gltf.at("meshes").at(meshIndex);
  //
  //    PSTC_CORE_INFO("  Mesh: {0}", gltfMesh.at("name"));
  //    for (const auto& primitive : gltfMesh.at("primitives"))
  //    {
  //      PSTC_CORE_INFO("    Attributes:");
  //      std::map<std::string, int> attributes = primitive.at("attributes").get<std::map<std::string, int>>();
  //      for (const auto& [name, accessorIndex] : attributes)
  //      {
  //        PSTC_CORE_INFO("      {0}", name);
  //
  //        auto accessor = gltf.at("accessors").at(accessorIndex);
  //        uint32_t bufferViewIndex = accessor.at("bufferView").get<uint32_t>();
  //        auto bufferView = gltf.at("bufferViews").at(bufferViewIndex);
  //        uint32_t bufferIndex = bufferView.at("buffer").get<uint32_t>();
  //        auto buffer = gltf.at("buffers").at(bufferIndex);
  //
  //        {
  //          PSTC_CORE_INFO("        Accessor");
  //
  //          PSTC_CORE_INFO("          byteOffset: {0}", accessor.at("byteOffset").get<uint32_t>());
  //          PSTC_CORE_INFO("          type: {0}", accessor.at("type"));
  //          PSTC_CORE_INFO("          componentType: {0}", accessor.at("componentType").get<uint32_t>());
  //          PSTC_CORE_INFO("          count: {0}", accessor.at("count").get<uint32_t>());
  //
  //          const std::string type = accessor.at("type").get<std::string>();
  //
  //          if (type == "SCALAR")
  //          {
  //            PSTC_CORE_INFO("          min: ({0})", accessor.at("min")[0].get<float>());
  //            PSTC_CORE_INFO("          max: ({0})", accessor.at("max")[0].get<float>());
  //          }
  //          else if (type == "VEC2")
  //          {
  //            PSTC_CORE_INFO("          min: ({0}, {1})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>());
  //            PSTC_CORE_INFO("          max: ({0}, {1})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>());
  //          }
  //          else if (type == "VEC3")
  //          {
  //            PSTC_CORE_INFO("          min: ({0}, {1}, {2})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>(), accessor.at("max")[2].get<float>());
  //            PSTC_CORE_INFO("          max: ({0}, {1}, {2})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>(), accessor.at("max")[2].get<float>());
  //          }
  //          else if (type == "VEC4")
  //          {
  //            PSTC_CORE_INFO("          min: ({0}, {1}, {2}, {3})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>(), accessor.at("max")[2].get<float>(), accessor.at("max")[3].get<float>());
  //            PSTC_CORE_INFO("          max: ({0}, {1}, {2}, {3})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>(), accessor.at("max")[2].get<float>(), accessor.at("max")[3].get<float>());
  //          }
  //        }
  //
  //        {
  //          PSTC_CORE_INFO("        BufferView");
  //          PSTC_CORE_INFO("          byteOffset: {0}", bufferView.at("byteOffset").get<uint32_t>());
  //          PSTC_CORE_INFO("          byteLength: {0}", bufferView.at("byteLength").get<uint32_t>());
  //
  //          if (bufferView.count("byteStride") > 0)
  //            PSTC_CORE_INFO("          byteStride: {0}", bufferView.at("byteStride").get<uint32_t>());
  //
  //          if (bufferView.count("target") > 0)
  //            PSTC_CORE_INFO("          target: {0}", bufferView.at("target").get<uint32_t>());
  //        }
  //
  //        {
  //          PSTC_CORE_INFO("        Buffer");
  //          PSTC_CORE_INFO("          byteLength: {0}", buffer.at("byteLength").get<uint32_t>());
  //          PSTC_CORE_INFO("          uri: {0}", buffer.at("uri").get<std::string>());
  //        }
  //
  //
  //      }
  //
  //      PSTC_CORE_INFO("    Indices");
  //      uint32_t indicesAcessorIndex = primitive.at("indices").get<uint32_t>();
  //      auto accessor = gltf.at("accessors").at(indicesAcessorIndex);
  //      uint32_t bufferViewIndex = accessor.at("bufferView").get<uint32_t>();
  //      auto bufferView = gltf.at("bufferViews").at(bufferViewIndex);
  //      uint32_t bufferIndex = bufferView.at("buffer").get<uint32_t>();
  //      auto buffer = gltf.at("buffers").at(bufferIndex);
  //
  //      {
  //        PSTC_CORE_INFO("        Accessor");
  //
  //        PSTC_CORE_INFO("          byteOffset: {0}", accessor.at("byteOffset").get<uint32_t>());
  //        PSTC_CORE_INFO("          type: {0}", accessor.at("type"));
  //        PSTC_CORE_INFO("          componentType: {0}", accessor.at("componentType").get<uint32_t>());
  //        PSTC_CORE_INFO("          count: {0}", accessor.at("count").get<uint32_t>());
  //
  //        const std::string type = accessor.at("type").get<std::string>();
  //
  //        if (type == "SCALAR")
  //        {
  //          PSTC_CORE_INFO("          min: ({0})", accessor.at("min")[0].get<float>());
  //          PSTC_CORE_INFO("          max: ({0})", accessor.at("max")[0].get<float>());
  //        }
  //        else if (type == "VEC2")
  //        {
  //          PSTC_CORE_INFO("          min: ({0}, {1})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>());
  //          PSTC_CORE_INFO("          max: ({0}, {1})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>());
  //        }
  //        else if (type == "VEC3")
  //        {
  //          PSTC_CORE_INFO("          min: ({0}, {1}, {2})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>(), accessor.at("max")[2].get<float>());
  //          PSTC_CORE_INFO("          max: ({0}, {1}, {2})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>(), accessor.at("max")[2].get<float>());
  //        }
  //        else if (type == "VEC4")
  //        {
  //          PSTC_CORE_INFO("          min: ({0}, {1}, {2}, {3})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>(), accessor.at("max")[2].get<float>(), accessor.at("max")[3].get<float>());
  //          PSTC_CORE_INFO("          max: ({0}, {1}, {2}, {3})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>(), accessor.at("max")[2].get<float>(), accessor.at("max")[3].get<float>());
  //        }
  //      }
  //
  //      {
  //        PSTC_CORE_INFO("        BufferView");
  //        PSTC_CORE_INFO("          byteOffset: {0}", bufferView.at("byteOffset").get<uint32_t>());
  //        PSTC_CORE_INFO("          byteLength: {0}", bufferView.at("byteLength").get<uint32_t>());
  //
  //        if (bufferView.count("byteStride") > 0)
  //          PSTC_CORE_INFO("          byteStride: {0}", bufferView.at("byteStride").get<uint32_t>());
  //
  //        if (bufferView.count("target") > 0)
  //          PSTC_CORE_INFO("          target: {0}", bufferView.at("target").get<uint32_t>());
  //      }
  //
  //      {
  //        PSTC_CORE_INFO("        Buffer");
  //        PSTC_CORE_INFO("          byteLength: {0}", buffer.at("byteLength").get<uint32_t>());
  //        PSTC_CORE_INFO("          uri: {0}", buffer.at("uri").get<std::string>());
  //      }
  //
  //      if (primitive.count("material") > 0) {
  //        uint32_t materialIndex = primitive.at("material").get<uint32_t>();
  //        auto material = gltf.at("materials")[materialIndex];
  //        PSTC_CORE_INFO("    Material: {0}", material.at("name"));
  //
  //        auto pbrMetallicRoughness = material.at("pbrMetallicRoughness");
  //
  //
  //
  //        uint32_t baseColorTextureIndex = pbrMetallicRoughness.at("baseColorTexture").at("index").get<uint32_t>();
  //        auto baseColorTexture = gltf.at("textures")[baseColorTextureIndex];
  //        uint32_t baseColorTextureSourceIndex = baseColorTexture.at("source");
  //        std::string colorTextureSource = gltf.at("images")[baseColorTextureSourceIndex].at("uri");
  //        PSTC_CORE_INFO("      Color Texture Source: {0}", colorTextureSource);
  //
  //        uint32_t baseColorTextureSamplerIndex = baseColorTexture.at("sampler");
  //        auto baseColorTextureSampler = gltf.at("samplers")[baseColorTextureSamplerIndex];
  //        // todo: parse sampler
  //
  //        if (pbrMetallicRoughness.at("baseColorTexture").count("texCoord"))
  //        {
  //          uint32_t baseColorTextureTexCoord = pbrMetallicRoughness.at("baseColorTexture").at("texCoord").get<uint32_t>();
  //          PSTC_CORE_INFO("        TexCoord: {0}", baseColorTextureTexCoord);
  //        }
  //
  //
  //
  //        if (pbrMetallicRoughness.count("baseColorFactor"))
  //        {
  //          glm::vec4 baseColorFactor = pbrMetallicRoughness.at("baseColorFactor");
  //          PSTC_CORE_INFO("    Base color factor: ({0}, {1}, {2}, {3})", baseColorFactor.x, baseColorFactor.y, baseColorFactor.z, baseColorFactor.w);
  //        }
  //
  //
  //
  //        uint32_t metallicRoughnessTextureIndex = pbrMetallicRoughness.at("metallicRoughnessTexture").at("index").get<uint32_t>();
  //        auto metallicRoughnessTexture = gltf.at("textures")[metallicRoughnessTextureIndex];
  //        uint32_t metallicRoughnessTextureSourceIndex = metallicRoughnessTexture.at("source");
  //        std::string metallicRoughnessTextureSource = gltf.at("images")[metallicRoughnessTextureSourceIndex].at("uri");
  //        PSTC_CORE_INFO("      Metallic Roughness Texture Source: {0}", metallicRoughnessTextureSource);
  //
  //        uint32_t metallicRoughnessTextureSamplerIndex = metallicRoughnessTexture.at("sampler");
  //        auto metallicRoughnessTextureSampler = gltf.at("samplers")[metallicRoughnessTextureSamplerIndex];
  //        // todo: parse sampler
  //
  //        if (pbrMetallicRoughness.at("metallicRoughnessTexture").count("texCoord"))
  //        {
  //          uint32_t metallicRoughnessTextureTexCoord = pbrMetallicRoughness.at("metallicRoughnessTexture").at("texCoord").get<uint32_t>();
  //          PSTC_CORE_INFO("        TexCoord: {0}", metallicRoughnessTextureTexCoord);
  //        }
  //
  //
  //        if (pbrMetallicRoughness.count("metallicFactor"))
  //          PSTC_CORE_INFO("      Metallic factor: {0}", pbrMetallicRoughness.at("metallicFactor").get<float>());
  //
  //        if (pbrMetallicRoughness.count("roughnessFactor"))
  //          PSTC_CORE_INFO("      Roughness factor: {0}", pbrMetallicRoughness.at("roughnessFactor").get<float>());
  //
  //        //todo: handle optional material data - normalTexture, occlusionTexture, emissiveTexture, emissiveFactor
  //
  //      }
  //
  //
  //      PSTC_CORE_INFO("    Mode: {0}", primitive.at("mode").get<uint32_t>());
  //
  //    }
  //  }
  //
  //  if (gltfNode.count("camera") > 0 && gltf.count("cameras") > 0)
  //  {
  //    uint32_t cameraIndex = gltfNode.at("camera");
  //    auto camera = gltf.at("cameras")[cameraIndex];
  //    std::string type = camera.at("type");
  //    
  //    if (type == "perspective")
  //    {
  //      PSTC_CORE_INFO("  Perspective Camera");
  //      PSTC_CORE_INFO("    aspectRatio: {0}", camera.at("aspectRatio").get<float>());
  //      PSTC_CORE_INFO("    yfov: {0}", camera.at("yfov").get<float>());
  //      PSTC_CORE_INFO("    zfar: {0}", camera.at("zfar").get<float>());
  //      PSTC_CORE_INFO("    znear: {0}", camera.at("znear").get<float>());
  //    }
  //
  //    if (type == "orthographic")
  //    {
  //      PSTC_CORE_INFO("  Orthographic Camera");
  //      PSTC_CORE_INFO("    xmag: {0}", camera.at("xmag").get<float>());
  //      PSTC_CORE_INFO("    ymag: {0}", camera.at("ymag").get<float>());
  //      PSTC_CORE_INFO("    zfar: {0}", camera.at("zfar").get<float>());
  //      PSTC_CORE_INFO("    znear: {0}", camera.at("znear").get<float>());
  //    }
  //
  //  }
  //  
  //  if (gltfNode.count("children") > 0)
  //    for (const auto& childIndex : gltfNode.at("children"))
  //      ProcessNode_Deprecated(gltf, childIndex, gltfNodeIndex);
  //
  //}


}

//namespace glm
//{
//  void from_json(const json& j, vec2& vec)
//  {
//    j.at("x").get_to(vec.x);
//    j.at("y").get_to(vec.y);
//  }
//
//  void from_json(const json& j, vec3& vec)
//  {
//    j.at("x").get_to(vec.x);
//    j.at("y").get_to(vec.y);
//    j.at("z").get_to(vec.z);
//  }
//
//  void from_json(const json& j, vec4& vec)
//  {
//    j.at("x").get_to(vec.x);
//    j.at("y").get_to(vec.y);
//    j.at("z").get_to(vec.z);
//    j.at("w").get_to(vec.w);
//  }
//
//  void from_json(const json& j, quat& vec)
//  {
//    j.at("x").get_to(vec.x);
//    j.at("y").get_to(vec.y);
//    j.at("z").get_to(vec.z);
//    j.at("w").get_to(vec.w);
//  }
//
//  void from_json(const json& json, mat4& mat)
//  {
//    auto jsonArray = json.array();
//
//    for (int i = 0; i < 4; i++)
//      for (int j = 0; j < 4; j++)
//        mat[i][j] = jsonArray[i + j * 4];
//
//  }
//}
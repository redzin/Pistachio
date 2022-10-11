#pragma once
#include "pch.h"
#include "SceneLoader.h"
#include <glm/gtx/quaternion.hpp>

namespace glm
{
  void from_json(const json& j, vec2& vec)
  {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
  }

  void from_json(const json& j, vec3& vec)
  {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
  }

  void from_json(const json& j, vec4& vec)
  {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
    j.at("w").get_to(vec.w);
  }

  void from_json(const json& j, quat& vec)
  {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
    j.at("w").get_to(vec.w);
  }

  void from_json(const json& json, mat4& mat)
  {
    auto jsonArray = json.array();

    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        mat[i][j] = jsonArray[i * 4 + j];
    
  }
}

namespace Pistachio
{
  void ProcessNode(const json& gltf, const uint32_t& gltfNodeIndex, const int32_t& parent = -1)
  {
    auto gltfNode = gltf.at("nodes")[gltfNodeIndex];
    if (gltfNode.count("name") > 0)
      PSTC_CORE_INFO("SceneNode: {0}", gltfNode.at("name"));
    else
      PSTC_CORE_INFO("SceneNode: {0}", gltfNodeIndex);

    glm::mat4 transform;
    if (gltfNode.count("matrix") > 0)
    {
      transform = gltfNode.at("matrix").get<glm::mat4>();
      PSTC_CORE_INFO("  Matrix loaded");
    }

    if (gltfNode.count("scale") > 0 && gltfNode.count("rotation") > 0 && gltfNode.count("translation") > 0)
    {
      // Scale
      glm::scale(transform, gltfNode.at("scale").get<glm::vec3>());

      // Rotate
      glm::quat quatRot = gltfNode.at("rotation").get<glm::quat>();
      glm::mat4 R = glm::toMat4(quatRot);
      transform = R * transform;

      // Translate
      glm::translate(transform, gltfNode.at("translation").get<glm::vec3>());
      PSTC_CORE_INFO("  Matrix loaded");
    }

    if (gltfNode.count("mesh") > 0 && gltf.count("meshes") > 0)
    {

      uint32_t meshIndex = gltfNode.at("mesh").get<uint32_t>();
      const auto& gltfMesh = gltf.at("meshes").at(meshIndex);

      PSTC_CORE_INFO("  Mesh: {0}", gltfMesh.at("name"));
      for (const auto& primitive : gltfMesh.at("primitives"))
      {
        PSTC_CORE_INFO("    Attributes:");
        std::map<std::string, int> attributes = primitive.at("attributes").get<std::map<std::string, int>>();
        for (const auto& [name, accessorIndex] : attributes)
        {
          PSTC_CORE_INFO("      {0}", name);

          auto accessor = gltf.at("accessors").at(accessorIndex);
          uint32_t bufferViewIndex = accessor.at("bufferView").get<uint32_t>();
          auto bufferView = gltf.at("bufferViews").at(bufferViewIndex);
          uint32_t bufferIndex = bufferView.at("buffer").get<uint32_t>();
          auto buffer = gltf.at("buffers").at(bufferIndex);

          {
            PSTC_CORE_INFO("        Accessor");

            PSTC_CORE_INFO("          byteOffset: {0}", accessor.at("byteOffset").get<uint32_t>());
            PSTC_CORE_INFO("          type: {0}", accessor.at("type"));
            PSTC_CORE_INFO("          componentType: {0}", accessor.at("componentType").get<uint32_t>());
            PSTC_CORE_INFO("          count: {0}", accessor.at("count").get<uint32_t>());

            const std::string type = accessor.at("type").get<std::string>();

            if (type == "SCALAR")
            {
              PSTC_CORE_INFO("          min: ({0})", accessor.at("min")[0].get<float>());
              PSTC_CORE_INFO("          max: ({0})", accessor.at("max")[0].get<float>());
            }
            else if (type == "VEC2")
            {
              PSTC_CORE_INFO("          min: ({0}, {1})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>());
              PSTC_CORE_INFO("          max: ({0}, {1})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>());
            }
            else if (type == "VEC3")
            {
              PSTC_CORE_INFO("          min: ({0}, {1}, {2})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>(), accessor.at("max")[2].get<float>());
              PSTC_CORE_INFO("          max: ({0}, {1}, {2})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>(), accessor.at("max")[2].get<float>());
            }
            else if (type == "VEC4")
            {
              PSTC_CORE_INFO("          min: ({0}, {1}, {2}, {3})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>(), accessor.at("max")[2].get<float>(), accessor.at("max")[3].get<float>());
              PSTC_CORE_INFO("          max: ({0}, {1}, {2}, {3})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>(), accessor.at("max")[2].get<float>(), accessor.at("max")[3].get<float>());
            }
          }

          {
            PSTC_CORE_INFO("        BufferView");
            PSTC_CORE_INFO("          byteOffset: {0}", bufferView.at("byteOffset").get<uint32_t>());
            PSTC_CORE_INFO("          byteLength: {0}", bufferView.at("byteLength").get<uint32_t>());

            if (bufferView.count("byteStride") > 0)
              PSTC_CORE_INFO("          byteStride: {0}", bufferView.at("byteStride").get<uint32_t>());

            if (bufferView.count("target") > 0)
              PSTC_CORE_INFO("          target: {0}", bufferView.at("target").get<uint32_t>());
          }

          {
            PSTC_CORE_INFO("        Buffer");
            PSTC_CORE_INFO("          byteLength: {0}", buffer.at("byteLength").get<uint32_t>());
            PSTC_CORE_INFO("          uri: {0}", buffer.at("uri").get<std::string>());
          }


        }

        PSTC_CORE_INFO("    Indices");
        uint32_t indicesAcessorIndex = primitive.at("indices").get<uint32_t>();
        auto accessor = gltf.at("accessors").at(indicesAcessorIndex);
        uint32_t bufferViewIndex = accessor.at("bufferView").get<uint32_t>();
        auto bufferView = gltf.at("bufferViews").at(bufferViewIndex);
        uint32_t bufferIndex = bufferView.at("buffer").get<uint32_t>();
        auto buffer = gltf.at("buffers").at(bufferIndex);

        {
          PSTC_CORE_INFO("        Accessor");

          PSTC_CORE_INFO("          byteOffset: {0}", accessor.at("byteOffset").get<uint32_t>());
          PSTC_CORE_INFO("          type: {0}", accessor.at("type"));
          PSTC_CORE_INFO("          componentType: {0}", accessor.at("componentType").get<uint32_t>());
          PSTC_CORE_INFO("          count: {0}", accessor.at("count").get<uint32_t>());

          const std::string type = accessor.at("type").get<std::string>();

          if (type == "SCALAR")
          {
            PSTC_CORE_INFO("          min: ({0})", accessor.at("min")[0].get<float>());
            PSTC_CORE_INFO("          max: ({0})", accessor.at("max")[0].get<float>());
          }
          else if (type == "VEC2")
          {
            PSTC_CORE_INFO("          min: ({0}, {1})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>());
            PSTC_CORE_INFO("          max: ({0}, {1})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>());
          }
          else if (type == "VEC3")
          {
            PSTC_CORE_INFO("          min: ({0}, {1}, {2})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>(), accessor.at("max")[2].get<float>());
            PSTC_CORE_INFO("          max: ({0}, {1}, {2})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>(), accessor.at("max")[2].get<float>());
          }
          else if (type == "VEC4")
          {
            PSTC_CORE_INFO("          min: ({0}, {1}, {2}, {3})", accessor.at("min")[0].get<float>(), accessor.at("min")[1].get<float>(), accessor.at("max")[2].get<float>(), accessor.at("max")[3].get<float>());
            PSTC_CORE_INFO("          max: ({0}, {1}, {2}, {3})", accessor.at("max")[0].get<float>(), accessor.at("max")[1].get<float>(), accessor.at("max")[2].get<float>(), accessor.at("max")[3].get<float>());
          }
        }

        {
          PSTC_CORE_INFO("        BufferView");
          PSTC_CORE_INFO("          byteOffset: {0}", bufferView.at("byteOffset").get<uint32_t>());
          PSTC_CORE_INFO("          byteLength: {0}", bufferView.at("byteLength").get<uint32_t>());

          if (bufferView.count("byteStride") > 0)
            PSTC_CORE_INFO("          byteStride: {0}", bufferView.at("byteStride").get<uint32_t>());

          if (bufferView.count("target") > 0)
            PSTC_CORE_INFO("          target: {0}", bufferView.at("target").get<uint32_t>());
        }

        {
          PSTC_CORE_INFO("        Buffer");
          PSTC_CORE_INFO("          byteLength: {0}", buffer.at("byteLength").get<uint32_t>());
          PSTC_CORE_INFO("          uri: {0}", buffer.at("uri").get<std::string>());
        }

        if (primitive.count("material") > 0) {
          uint32_t materialIndex = primitive.at("material").get<uint32_t>();
          auto material = gltf.at("materials")[materialIndex];
          PSTC_CORE_INFO("    Material: {0}", material.at("name"));

          auto pbrMetallicRoughness = material.at("pbrMetallicRoughness");



          uint32_t baseColorTextureIndex = pbrMetallicRoughness.at("baseColorTexture").at("index").get<uint32_t>();
          auto baseColorTexture = gltf.at("textures")[baseColorTextureIndex];
          uint32_t baseColorTextureSourceIndex = baseColorTexture.at("source");
          std::string colorTextureSource = gltf.at("images")[baseColorTextureSourceIndex].at("uri");
          PSTC_CORE_INFO("      Color Texture Source: {0}", colorTextureSource);

          uint32_t baseColorTextureSamplerIndex = baseColorTexture.at("sampler");
          auto baseColorTextureSampler = gltf.at("samplers")[baseColorTextureSamplerIndex];
          // todo: parse sampler

          if (pbrMetallicRoughness.at("baseColorTexture").count("texCoord"))
          {
            uint32_t baseColorTextureTexCoord = pbrMetallicRoughness.at("baseColorTexture").at("texCoord").get<uint32_t>();
            PSTC_CORE_INFO("        TexCoord: {0}", baseColorTextureTexCoord);
          }



          if (pbrMetallicRoughness.count("baseColorFactor"))
          {
            glm::vec4 baseColorFactor = pbrMetallicRoughness.at("baseColorFactor");
            PSTC_CORE_INFO("    Base color factor: ({0}, {1}, {2}, {3})", baseColorFactor.x, baseColorFactor.y, baseColorFactor.z, baseColorFactor.w);
          }



          uint32_t metallicRoughnessTextureIndex = pbrMetallicRoughness.at("metallicRoughnessTexture").at("index").get<uint32_t>();
          auto metallicRoughnessTexture = gltf.at("textures")[metallicRoughnessTextureIndex];
          uint32_t metallicRoughnessTextureSourceIndex = metallicRoughnessTexture.at("source");
          std::string metallicRoughnessTextureSource = gltf.at("images")[metallicRoughnessTextureSourceIndex].at("uri");
          PSTC_CORE_INFO("      Metallic Roughness Texture Source: {0}", metallicRoughnessTextureSource);

          uint32_t metallicRoughnessTextureSamplerIndex = metallicRoughnessTexture.at("sampler");
          auto metallicRoughnessTextureSampler = gltf.at("samplers")[metallicRoughnessTextureSamplerIndex];
          // todo: parse sampler

          if (pbrMetallicRoughness.at("metallicRoughnessTexture").count("texCoord"))
          {
            uint32_t metallicRoughnessTextureTexCoord = pbrMetallicRoughness.at("metallicRoughnessTexture").at("texCoord").get<uint32_t>();
            PSTC_CORE_INFO("        TexCoord: {0}", metallicRoughnessTextureTexCoord);
          }


          if (pbrMetallicRoughness.count("metallicFactor"))
            PSTC_CORE_INFO("      Metallic factor: {0}", pbrMetallicRoughness.at("metallicFactor").get<float>());

          if (pbrMetallicRoughness.count("roughnessFactor"))
            PSTC_CORE_INFO("      Roughness factor: {0}", pbrMetallicRoughness.at("roughnessFactor").get<float>());

          //todo: handle optional material data - normalTexture, occlusionTexture, emissiveTexture, emissiveFactor

        }


        PSTC_CORE_INFO("    Mode: {0}", primitive.at("mode").get<uint32_t>());

      }
    }

    if (gltfNode.count("camera") > 0 && gltf.count("cameras") > 0)
    {
      uint32_t cameraIndex = gltfNode.at("camera");
      auto camera = gltf.at("cameras")[cameraIndex];
      std::string type = camera.at("type");
      
      if (type == "perspective")
      {
        PSTC_CORE_INFO("  Perspective Camera");
        PSTC_CORE_INFO("    aspectRatio: {0}", camera.at("aspectRatio").get<float>());
        PSTC_CORE_INFO("    yfov: {0}", camera.at("yfov").get<float>());
        PSTC_CORE_INFO("    zfar: {0}", camera.at("zfar").get<float>());
        PSTC_CORE_INFO("    znear: {0}", camera.at("znear").get<float>());
      }

      if (type == "orthographic")
      {
        PSTC_CORE_INFO("  Orthographic Camera");
        PSTC_CORE_INFO("    xmag: {0}", camera.at("xmag").get<float>());
        PSTC_CORE_INFO("    ymag: {0}", camera.at("ymag").get<float>());
        PSTC_CORE_INFO("    zfar: {0}", camera.at("zfar").get<float>());
        PSTC_CORE_INFO("    znear: {0}", camera.at("znear").get<float>());
      }

    }
    
    if (gltfNode.count("children") > 0)
      for (const auto& childIndex : gltfNode.at("children"))
        ProcessNode(gltf, childIndex, gltfNodeIndex);

  }

  Scene LoadScene(const std::string& path)
  {
    std::string f = ReadFile(path);

    json gltf = json::parse(f);

    Scene scene;

    for (const auto& scene : gltf.at("scenes"))
    {

      for (const uint32_t& gltfNodeIndex : scene.at("nodes"))
      {
        ProcessNode(gltf, gltfNodeIndex);
      }

    }

    return scene;

  }

}
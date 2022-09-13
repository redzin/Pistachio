#pragma once
#include "pch.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Pistacio
{
  
  // What is model? Mesh + Material (divided into submeshes with individual materials, e.g. wood, metal, cloth, hair, etc.)
  // What is Mesh? Series of vertex buffers - positions, normals, texture coords, etc.
  // What is Material? Textures (base color, roughness, bump map, etc.)

  Scene LoadScene(const std::string& path = "../../Pistacio/vendor/tinygltf/tinygltf/models/Cube/Cube.gltf");
  

}
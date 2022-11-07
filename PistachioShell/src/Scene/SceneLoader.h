#pragma once
#include "pch.h"
#include "Scene.h"
#include "SceneComponents.h"
#include "Camera/Camera.h"
#include "Camera/CameraOrbitController.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Pistachio
{
  
  // Model: Mesh + Material (divided into submeshes with individual materials, e.g. wood, metal, cloth, hair, etc.)
  // Mesh: Series of vertex buffers - positions, normals, texture coords, etc.
  // Material: Textures (base color, roughness, bump map, etc.)

  class SceneLoader
  {
  public:
    //std::vector<Scene> LoadGLTF(Device& device, const std::string& path = "vendor/gltf-Sample-Models/2.0/FlightHelmet/glTF/FlightHelmet.gltf");
    //std::vector<Scene> LoadGLTF(Device& device, const std::string& path = "vendor/gltf-Sample-Models/2.0/MetalRoughSpheresNoTextures/glTF/MetalRoughSpheresNoTextures.gltf");
    std::vector<Scene> LoadGLTF(Device& device, const std::string& path = "vendor/gltf-Sample-Models/2.0/MetalRoughSpheres/glTF/MetalRoughSpheres.gltf");
    //std::vector<Scene> LoadGLTF(Device& device, const std::string& path = "vendor/gltf-Sample-Models/2.0/Duck/glTF/Duck.gltf");
    //std::vector<Scene> LoadGLTF(Device& device, const std::string& path = "vendor/gltf-Sample-Models/2.0/Cube/glTF/Cube.gltf");
    //std::vector<Scene> LoadGLTF(Device& device, const std::string& path = "vendor/gltf-Sample-Models/2.0/Box/glTF/Box.gltf");

  private:

  };

  

}
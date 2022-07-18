#pragma once

#include "pch.h"
#include "Renderer.h"

namespace Pistacio
{

  class Shader
  {
  public:
    virtual ~Shader() = default;

    virtual const RendererID GetRendererID() const = 0;
    virtual const std::string& GetName() const = 0;

    static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    static Ref<Shader> Create(const std::string& filePath);
  private:
    uint32_t ShaderProgramId;
  };

  class ShaderLibrary
  {
  public:
    ShaderLibrary() = default;
    ~ShaderLibrary() = default;
    void Add(const Ref<Shader>& shader);
    void Add(const std::string& name, const Ref<Shader>& shader);
    Ref<Shader> Load(const std::string& path);
    Ref<Shader> Load(const std::string& name, const std::string& path);

    Ref<Shader> Get(const std::string& name);
    bool Exists(const std::string& name);

  private:
    std::unordered_map<std::string, Ref<Shader>> shaders;
  };

}


#include "pch.h"
#include "Shader.h"

#include "PistacioPlatform/OpenGL/Shader_OpenGL.h"

namespace Pistacio
{
  Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<Shader_OpenGL>(name, vertexSrc, fragmentSrc);
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }

  Ref<Shader> Shader::Create(const std::string& filePath)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<Shader_OpenGL>(filePath);
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }

  void ShaderLibrary::Add(const Ref<Shader>& shader)
  {
    auto& name = shader->GetName();
    Add(name, shader);
  }

  void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
  {
    PSTC_CORE_ASSERT(!Exists(name), "Shader already exists!");
    shaders[name] = shader;
  }

  Ref<Shader> ShaderLibrary::Load(const std::string& path)
  {
    auto shader = Shader::Create(path);
    Add(shader);
    return shader;
  }

  Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& path)
  {
    auto shader = Shader::Create(path);
    Add(name, shader);
    return shader;
  }

  Ref<Shader> ShaderLibrary::Get(const std::string& name)
  {

    PSTC_CORE_ASSERT(Exists(name), "Shader not found!");
    return shaders[name];

  }

  bool ShaderLibrary::Exists(const std::string& name)
  {
    return shaders.find(name) != shaders.end();
  }

}
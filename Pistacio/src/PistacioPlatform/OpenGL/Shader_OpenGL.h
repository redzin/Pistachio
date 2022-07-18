#pragma once

#include "Pistacio/Rendering/Shader.h"

namespace Pistacio
{
  class Shader_OpenGL : public Shader
  {
  public:
    Shader_OpenGL(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    Shader_OpenGL(const std::string& filePath);
    virtual ~Shader_OpenGL();

    const std::string& GetName() const override;
    const RendererID GetRendererID() const override;
    
  private:
    std::string ReadFile(const std::string& path);
    std::vector<GLenum> GetShaderTypes(const std::string& path);
    void Compile(const std::string& shaderSrc, std::vector<GLenum>& types);
  private:
    std::string Name;
    RendererID m_RendererId;
  };
}


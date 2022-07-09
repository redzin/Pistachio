#pragma once

#include "Pistacio/Rendering/Shader.h"

namespace Pistacio
{
  class Shader_OpenGL : public Shader
  {
  public:
    Shader_OpenGL(std::string vertexSrc, std::string fragmentSrc);
    virtual ~Shader_OpenGL();
    void Bind() const override;
    void Unbind() const override;
    
  private:
    unsigned int RendererID;
  };
}


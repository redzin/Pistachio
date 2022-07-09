#pragma once

#include "pch.h"
#include "Renderer.h"

namespace Pistacio
{

  class Shader
  {
  public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static Shader* Create(std::string& vertexSrc, std::string& fragmentSrc);
  private:
    uint32_t ShaderProgramId;
  };

}


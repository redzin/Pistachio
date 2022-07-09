#include "pch.h"
#include "Shader.h"

#include "PistacioPlatform/OpenGL/Shader_OpenGL.h"

namespace Pistacio
{
  Shader* Shader::Create(std::string& vertexSrc, std::string& fragmentSrc)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new Shader_OpenGL(vertexSrc, fragmentSrc);
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }
}
#include "pch.h"
#include "UniformBuffer.h"

#include "PistacioPlatform/OpenGL/UniformBuffer_OpenGL.h"

namespace Pistacio
{
  UniformBuffer* Pistacio::UniformBuffer::Create(uint32_t size, uint32_t binding)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new UniformBuffer_OpenGL(size, binding);
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }
}



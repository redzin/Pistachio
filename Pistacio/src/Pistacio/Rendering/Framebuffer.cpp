#include "pch.h"
#include "Framebuffer.h"

#include "PistacioPlatform/OpenGL/Framebuffer_OpenGL.h"

namespace Pistacio
{

  Ref<Framebuffer> Framebuffer::Create(FramebufferDescriptor descriptor)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<Framebuffer_OpenGL>(descriptor);
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;


  }
}

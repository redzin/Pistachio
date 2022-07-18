#include "pch.h"
#include "Texture.h"

#include "PistacioPlatform/OpenGL/Texture2D_OpenGL.h"

namespace Pistacio
{

  Ref<Texture2D> Texture2D::Create(const std::string& path, TextureDescriptor descriptor)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;

    case RendererAPI::API::OpenGL:
      Ref<Texture2D> ref = nullptr;
      ref.reset(new Texture2D_OpenGL(path, descriptor));
      return ref;
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }

  Ref<Texture2D> Texture2D::Create(TextureDescriptor descriptor)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;

    case RendererAPI::API::OpenGL:
      Ref<Texture2D> ref = nullptr;
      ref.reset(new Texture2D_OpenGL(descriptor));
      return ref;
    }
  
    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }

}
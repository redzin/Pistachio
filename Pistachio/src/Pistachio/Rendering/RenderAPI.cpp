#include "pch.h"
#include "RenderAPI.h"

#include "PistachioPlatform/OpenGL/RenderingAPI_OpenGL.h"

namespace Pistachio
{
  RenderingAPI::API RenderingAPI::m_API = RenderingAPI::API::OpenGL;

  Ref<RenderingAPI> RenderingAPI::Create()
  {
    switch (RenderingAPI::GetAPI())
    {
    case RenderingAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RenderingAPI::API::OpenGL:
      return Ref<RenderingAPI>(new RenderingAPI_OpenGL());
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }

}

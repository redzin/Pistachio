#include "pch.h"
#include "VertexArray.h"

#include "PistacioPlatform/OpenGL/VertexArray_OpenGL.h"

namespace Pistacio
{

  Ref<VertexArray> Pistacio::VertexArray::Create()
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      Ref<VertexArray> ref;
      ref.reset(new VertexArray_OpenGL());
      return ref;
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }

}

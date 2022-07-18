#include "pch.h"
#include "Buffer.h"

#include "PistacioPlatform/OpenGL/Buffer_OpenGL.h"

namespace Pistacio
{

  // Vertex Buffer //////////////////////////////////////////////////
  Ref<VertexBuffer> Pistacio::VertexBuffer::Create(float* vertices, uint32_t size)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      Ref<VertexBuffer> ref;
      ref.reset(new VertexBuffer_OpenGL(vertices, size));
      return ref;
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }

  // Index Buffer //////////////////////////////////////////////////
  Ref<IndexBuffer> IndexBuffer::Create(uint32_t* vertices, uint32_t size)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      Ref<IndexBuffer> ref;
      ref.reset(new IndexBuffer_OpenGL(vertices, size));
      return ref;
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }

}

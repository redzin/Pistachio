#include "pch.h"
#include "Buffer.h"

#include "PistacioPlatform/OpenGL/Buffer_OpenGL.h"

namespace Pistacio
{

  // Vertex Buffer //////////////////////////////////////////////////
  VertexBuffer* Pistacio::VertexBuffer::Create(float* vertices, uint32_t size)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new VertexBuffer_OpenGL(vertices, size);
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }

  // Index Buffer //////////////////////////////////////////////////
  IndexBuffer* IndexBuffer::Create(uint32_t* vertices, uint32_t size)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::None():
      PSTC_CORE_ASSERT(false, "No renderer API selected!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new IndexBuffer_OpenGL(vertices, size);
    }

    PSTC_CORE_ASSERT(false, "Unknown API selected!");
    return nullptr;
  }

}

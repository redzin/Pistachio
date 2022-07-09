#include "pch.h"
#include "RendererAPI_OpenGL.h"

namespace Pistacio
{

  void RendererAPI_OpenGL::SetClearColor(const glm::vec4& color)
  {
    glClearColor(color.r, color.g, color.b, color.a);
  }

  void RendererAPI_OpenGL::Clear()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void RendererAPI_OpenGL::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
  {
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
  }

}


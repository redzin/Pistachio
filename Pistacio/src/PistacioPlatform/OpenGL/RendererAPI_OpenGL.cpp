#include "pch.h"
#include "RendererAPI_OpenGL.h"

namespace Pistacio
{

  void RendererAPI_OpenGL::SetClearColor(const glm::vec4& color)
  {
    glClearColor(color.r, color.g, color.b, color.a);
  }

  void RendererAPI_OpenGL::SetClearDepth(const float depth)
  {
    glClearDepth(depth);
  }

  void RendererAPI_OpenGL::Clear()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void RendererAPI_OpenGL::Enable(Capability capability)
  {
    glEnable(static_cast<GLenum>(capability));
  }

  void RendererAPI_OpenGL::Enable(std::initializer_list<Capability> capabilities)
  {
    for (auto capability : capabilities)
    {
      glEnable(static_cast<GLenum>(capability));
    }
  }

  void RendererAPI_OpenGL::Disable(Capability capability)
  {
    glDisable(static_cast<GLenum>(capability));
  }

  void RendererAPI_OpenGL::Disable(std::initializer_list<Capability> capabilities)
  {
    for (auto capability : capabilities)
    {
      glDisable(static_cast<GLenum>(capability));
    }
  }


  void RendererAPI_OpenGL::DepthMask(bool enable)
  {
    glDepthMask(enable);
  }

  void RendererAPI_OpenGL::SetDepthFunc(DepthFunction depthFunc)
  {
    glDepthFunc(static_cast<GLenum>(depthFunc));
  }


  void RendererAPI_OpenGL::SetBlendFunction(BlendFunction sfactor, BlendFunction dfactor)
  {
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(static_cast<GLenum>(sfactor), static_cast<GLenum>(dfactor));
  }

  void RendererAPI_OpenGL::DrawIndexed(const Ref<VertexArray>& vertexArray)
  {
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
  }

  void RendererAPI_OpenGL::Bind(const Ref<VertexArray>& vertexArray) const
  {
    glBindVertexArray(vertexArray->GetRendererID());
  }

  void RendererAPI_OpenGL::Bind(const Ref<Shader>& shader) const
  {
    glUseProgram(shader->GetRendererID());
  }

  void RendererAPI_OpenGL::Bind(const Ref<IndexBuffer>& buffer) const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->GetRendererID());
  }

  void RendererAPI_OpenGL::Bind(const Ref<VertexBuffer>& buffer) const
  {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->GetRendererID());
  }

  void RendererAPI_OpenGL::Bind(const Ref<Texture2D> texture, uint32_t unit) const
  {
    glBindTextureUnit(unit, texture->GetRendererID());
  }

  void RendererAPI_OpenGL::Bind(const Ref<Framebuffer>& framebuffer) const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->GetRendererID());
  }

  void RendererAPI_OpenGL::SetViewport(int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height)
  {
    glViewport(xoffset, yoffset, width, height);
  };
}


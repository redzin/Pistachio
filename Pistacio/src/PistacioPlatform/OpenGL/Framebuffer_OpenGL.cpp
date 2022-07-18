#include "pch.h"
#include "Framebuffer_OpenGL.h"

namespace Pistacio
{



  Framebuffer_OpenGL::Framebuffer_OpenGL(FramebufferDescriptor descriptor) : m_Descriptor(descriptor)
  {
    if (descriptor.IsSwapchainTarget)
    {
      m_RendererId = 0;
    }
    else
    {
      glCreateFramebuffers(1, &m_RendererId);
      Framebuffer_OpenGL::Resize(descriptor.Width, descriptor.Height);
    }
  }

  Framebuffer_OpenGL::~Framebuffer_OpenGL()
  {
    glDeleteFramebuffers(GL_FRAMEBUFFER, &m_RendererId);
  }

  //void Framebuffer_OpenGL::Bind() const
  //{
  //  glBindFramebuffer(GL_FRAMEBUFFER, RendererID);
  //}
  //
  //void Framebuffer_OpenGL::Unbind() const
  //{
  //  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //}

  Ref<Texture2D> Framebuffer_OpenGL::GetColorAttachment() const
  {
    return m_ColorTexture;
  }

  Ref<Texture2D> Framebuffer_OpenGL::GetDepthAttachment() const
  {
    return m_DepthTexture;
  }

  const FramebufferDescriptor& Framebuffer_OpenGL::GetDescriptor() const
  {
    return m_Descriptor;
  }

  void Framebuffer_OpenGL::Resize(uint32_t width, uint32_t height)
  {
    if (m_RendererId == 0)
      return;

    TextureDescriptor colorTextureDesc{};
    colorTextureDesc.Width = width;
    colorTextureDesc.Height = height;
    colorTextureDesc.InternalFormat = TextureGPUInternalFormat::RGB8;

    TextureDescriptor depthTextureDesc{};
    depthTextureDesc.Width = width;
    depthTextureDesc.Height = height;
    depthTextureDesc.InternalFormat = TextureGPUInternalFormat::DepthComponent16;

    m_ColorTexture = Texture2D::Create(colorTextureDesc);
    m_DepthTexture = Texture2D::Create(depthTextureDesc);

    glNamedFramebufferTexture(m_RendererId, GL_COLOR_ATTACHMENT0, m_ColorTexture->GetRendererID(), 0);
    glNamedFramebufferTexture(m_RendererId, GL_DEPTH_ATTACHMENT, m_DepthTexture->GetRendererID(), 0);

    GLenum framebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    PSTC_CORE_ASSERT(framebufferComplete == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");

  }

  const RendererID Framebuffer_OpenGL::GetRendererID() const
  {
    return m_RendererId;
  }

}

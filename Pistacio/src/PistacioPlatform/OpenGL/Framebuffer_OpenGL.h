#pragma once
#include "pch.h"
#include "Pistacio/Rendering/Framebuffer.h"


namespace Pistacio
{
  class Framebuffer_OpenGL : public Framebuffer
  {
  public:
    Framebuffer_OpenGL(FramebufferDescriptor descriptor);
    ~Framebuffer_OpenGL();

    Ref<Texture2D> GetColorAttachment() const override;
    Ref<Texture2D> GetDepthAttachment() const override;
    
    const FramebufferDescriptor& GetDescriptor() const override;
    const RendererID GetRendererID() const override;

    void Resize(uint32_t width, uint32_t height) override;

  private:
    FramebufferDescriptor m_Descriptor;
    RendererID m_RendererId = 0;
    Ref<Texture2D> m_ColorTexture;
    Ref<Texture2D> m_DepthTexture;

  };

}



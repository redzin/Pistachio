#pragma once
#include "Texture.h"
#include <variant>

namespace Pistacio
{

  
  struct FramebufferDescriptor
  {
    uint32_t Width, Height;
    uint32_t Samples = 1;
    bool IsSwapchainTarget = false;
  };

  class Framebuffer
  {
  public:
    virtual Ref<Texture2D> GetColorAttachment() const = 0;
    virtual Ref<Texture2D> GetDepthAttachment() const = 0;
    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual const FramebufferDescriptor& GetDescriptor() const = 0;
    virtual const RendererID GetRendererID() const = 0;

    static Ref<Framebuffer> Create(FramebufferDescriptor descriptor);
    
    virtual ~Framebuffer() = default;
  private:
  };
}


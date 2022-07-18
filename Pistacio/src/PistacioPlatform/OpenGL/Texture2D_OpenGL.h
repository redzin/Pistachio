#pragma once
#include "pch.h"

#include "Pistacio/Rendering/Texture.h"

namespace Pistacio
{
  class Texture2D_OpenGL : public Texture2D
  {
  public:
    ~Texture2D_OpenGL() {}
    Texture2D_OpenGL(const std::string& path, TextureDescriptor descriptor);
    Texture2D_OpenGL(TextureDescriptor descriptor);

    TextureDescriptor GetDescriptor() const override { return Descriptor; }
    const RendererID GetRendererID() const override;
    void SetData(Image image) const override;

  private:
    TextureDescriptor Descriptor;
    RendererID m_RendererId;
  };
  

}
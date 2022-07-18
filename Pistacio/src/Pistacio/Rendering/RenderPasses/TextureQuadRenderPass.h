#pragma once

namespace Pistacio
{
  class TextureQuadRenderPass
  {
  public:
    TextureQuadRenderPass();
    ~TextureQuadRenderPass() = default;

    void Render(Ref<Texture2D> texture, Ref<Framebuffer> target) const;

  private:
    Ref<VertexArray> m_VertexArray;
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<IndexBuffer> m_IndexBuffer;

  };
}



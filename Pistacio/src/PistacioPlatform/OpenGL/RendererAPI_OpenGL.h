#pragma once

#include "Pistacio/Rendering/Renderer.h"

namespace Pistacio
{

  class RendererAPI_OpenGL : public RendererAPI
  {
  public:

    virtual void Clear() override;

    void Enable(Capability capability) override;
    void Enable(std::initializer_list<Capability> capabilities) override;
    void Disable(Capability capability) override;
    void Disable(std::initializer_list<Capability> capabilities) override;
    void DepthMask(bool enable = true) override;
    void SetDepthFunc(DepthFunction depthFunc) override;
    void SetBlendFunction(BlendFunction sfactor, BlendFunction dfactor) override;
    void SetViewport(int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height) override;
    void SetClearColor(const glm::vec4& color) override;
    void SetClearDepth(const float depth) override;

    void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

    void Bind(const Ref<VertexArray>& vertexArray) const override;
    void Bind(const Ref<Shader>& shader) const override;
    void Bind(const Ref<IndexBuffer>& buffer) const override;
    void Bind(const Ref<VertexBuffer>& buffer) const override;
    void Bind(const Ref<Texture2D> texture, uint32_t unit) const override;
    void Bind(const Ref<Framebuffer>& framebuffer) const override;
  };

}

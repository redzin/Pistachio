#pragma once
#include "pch.h"

#include "Buffer.h"
#include "VertexArray.h"
#include "Framebuffer.h"
#include "Shader.h"

namespace Pistacio
{

  class RendererAPI
  {
  public:


    enum class API
    {
      None = 0, OpenGL = 1
    };

    enum class DepthFunction
    {
      Never = 0x0200,
      Less = 0x0201,
      Equal = 0x0202,
      LEqual = 0x0203,
      Greater = 0x0204,
      NotEqual = 0x0205,
      GEqual = 0x0206,
      Always = 0x0207
    };

    enum class BlendFunction
    {
      Zero = 0,
      One = 1,
      SrcColor = 0x0300,
      OneMinusSrcColor = 0x0301,
      SrcAlpha = 0x0302,
      OneMinusSrcAlpha = 0x0303,
      DstAlpha = 0x0304,
      OneMinusDstAlpha = 0x0305,
      DstColor = 0x0306,
      OneMinusDstColor = 0x0307,
      SrcAlphaSaturate = 0x0308
    };

    enum class Capability
    {
      CullFace = 0x0B44,
      DepthTest = 0x0B71,
      StencilTest = 0x0B90,
      Blend = 0x0BE2
    };

    virtual void Clear() = 0;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

    virtual void Enable(Capability capability) = 0;
    virtual void Enable(std::initializer_list<Capability> capabilities) = 0;
    virtual void Disable(Capability capability) = 0;
    virtual void Disable(std::initializer_list<Capability> capabilities) = 0;
    virtual void DepthMask(bool enable = true) = 0;
    virtual void SetDepthFunc(DepthFunction depthFunc) = 0;
    virtual void SetBlendFunction(BlendFunction sfactor, BlendFunction dfactor) = 0;
    virtual void SetViewport(int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void SetClearDepth(const float depth) = 0;

    virtual void Bind(const Ref<VertexArray>& vertexArray) const = 0;
    virtual void Bind(const Ref<Shader>& shader) const = 0;
    virtual void Bind(const Ref<IndexBuffer>& buffer) const = 0;
    virtual void Bind(const Ref<VertexBuffer>& buffer) const = 0;
    virtual void Bind(const Ref<Texture2D> texture, uint32_t unit) const = 0;
    virtual void Bind(const Ref<Framebuffer>& framebuffer) const = 0;

    inline static API GetAPI() { return api; }
    inline static Scope<RendererAPI>& Get() { return Instance; }
  private:
    static Scope<RendererAPI> Instance;
    static API api;
  };

}
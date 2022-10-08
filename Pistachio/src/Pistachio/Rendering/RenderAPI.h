#pragma once
#include "pch.h"

namespace Pistachio
{

  class RenderingAPI
  {
  public:


    enum class API
    {
      None = 0, OpenGL = 1
    };

    typedef void* (*GetRenderAPIProcAddress)(const char* SemanticName);
    virtual int InitExternalProcAddress(GetRenderAPIProcAddress addressGetPtr) const = 0;
    virtual void DrawIndexed(const RendererID& inputLayoutID, uint32_t indexCount, uint32_t primitiveType = 0x0004, uint32_t indexBufferBaseType = 0x1405, uint32_t Offset = 0) const = 0; // indexBufferBaseType default is GL_UNSIGNED_INT, primitive default is GL_TRIANGLES
    virtual void DrawIndexedInstanced(const RendererID& inputLayoutID, uint32_t indexCount, uint32_t instanceCount, uint32_t primitiveType = 0x0004, uint32_t indexBufferBaseType = 0x1405, uint32_t Offset = 0) const = 0; // indexBufferBaseType default is GL_UNSIGNED_INT, primitive default is GL_TRIANGLES
    virtual void Draw(const RendererID& inputLayoutID, uint32_t vertexCount, uint32_t primitiveType = 0x0004, uint32_t Offset = 0) const = 0; // see comment in line above
    virtual void Clear(FramebufferMask mask) const = 0;

    virtual void Enable(Capability capability) const = 0;
    virtual void Enable(std::initializer_list<Capability> capabilities) const = 0;
    virtual void EnableDebugOutput() const = 0;
    virtual void Disable(Capability capability) const = 0;
    virtual void Disable(std::initializer_list<Capability> capabilities) const = 0;
    virtual void DepthMask(bool enable = true) const = 0;
    virtual void SetDepthFunc(DepthFunction depthFunc) const = 0;
    virtual void SetBlendFunction(BlendFunction sfactor, BlendFunction dfactor) const = 0;
    virtual void SetViewport(int32_t xoffset, int32_t yoffset, uint32_t m_Width, uint32_t m_Height) const = 0;
    virtual void SetClearColor(const glm::vec4& color) const = 0;
    virtual void SetClearDepth(const float depth) const = 0;
    virtual void SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const = 0;

    virtual RendererID CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) const = 0;
    virtual RendererID CreateShader(const std::string& path) const = 0;
    virtual void DeleteShader(RendererID& shader) const = 0;
    virtual void UseShaderProgram(const RendererID& shader) const = 0;

    //virtual RendererID CreateBuffer(const BufferDescriptor& descriptor) const = 0;
    //virtual RendererID CreateBuffer(const BufferDescriptor& descriptor, const void* data) const = 0;
    virtual RendererID CreatePersistenBuffer(const BufferDescriptor descriptor) const = 0;
    virtual RendererID CreatePersistenBuffer(const BufferDescriptor descriptor, const void* data) const = 0;
    virtual void* MapBuffer(RendererID rendererId, uint32_t offset, uint32_t size, uint32_t accessFlags) const = 0;
    virtual void UnmapBuffer(RendererID rendererId) const = 0;
    //virtual void UploadBufferData(const RendererID& rendererId, const BufferDescriptor& buffer, uint32_t Offset, const void* data) const = 0;
    //virtual void UploadBufferData(const RendererID& rendererId, uint32_t size, uint32_t offset, const void* data) const = 0;
    virtual void DeleteBuffer(RendererID& rendererId) const = 0;
    virtual void SetBufferUniformBinding(const RendererID& buffer, const uint32_t& binding) const = 0;

    virtual RendererID CreateAttributeLayout(const AttributeLayoutDescriptor& attributeDescriptor) const = 0;
    virtual RendererID CreateAttributeLayout(RendererID indexBuffer, const AttributeLayoutDescriptor& attributeDescriptor) const = 0;
    virtual void DeleteInputLayout(RendererID& id) const = 0;

    virtual RendererID CreateSampler(const SamplerDescriptor& samplerDescriptor) const = 0;
    virtual RendererID CreateSampler(const std::string& path, SamplerDescriptor& samplerDescriptor) const = 0;
    virtual void UploadSamplerData(const Sampler& sampler, const Image& image, const uint32_t layer = 0) const = 0;
    virtual void BindSampler(const RendererID& rendererId, uint32_t unit) const = 0;
    virtual void DeleteSampler(RendererID& id) const = 0;

    virtual RendererID CreateAttachment(const TextureDescriptorBase& descriptor) const = 0;
    virtual void DeleteAttachment(RendererID& id) const = 0;
    virtual RendererID CreateFramebuffer() const = 0;
    virtual void DeleteFramebuffer(RendererID& rendererId) const = 0;
    virtual void CheckFramebufferCompleteness(const RendererID& framebufferId) const = 0;
    virtual void InvalidateFramebuffer(const RendererID framebufferId, uint32_t invalidAttachmentCount, const AttachmentPoint* attachmentPointArray) const = 0;
    virtual void BindAttachment(const RendererID& framebufferId, const RendererID& attachmentId, const AttachmentPoint& attachmentPoint) const = 0;
    virtual void BindFramebuffer(const RendererID& rendererId) const = 0;
    virtual void UnbindFramebuffer() const = 0;
    virtual void BlitFramebuffer(
      const RendererID& sourceId, uint32_t sourceWidth, uint32_t sourceHeight,
      const RendererID& destId, uint32_t destWidth, uint32_t destHeight,
      TextureFilter filter = FILTER_NEAREST, FramebufferMask mask = COLOR_BUFFER) const = 0;

    //virtual void SetWindowPtr(const Ref<Window>& window) = 0;

    static constexpr uint32_t DefaultWidth = 1280;
    static constexpr uint32_t DefaultHeight = 720;

    static API GetAPI() { return m_API; }
    static Ref<RenderingAPI> Create();
  private:
    static API m_API;
  };

}
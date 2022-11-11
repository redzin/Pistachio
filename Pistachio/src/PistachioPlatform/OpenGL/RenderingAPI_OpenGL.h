#pragma once

namespace Pistachio
{

  class RenderingAPI_OpenGL : public RenderingAPI
  {
  public:
    int InitExternalProcAddress(GetRenderAPIProcAddress addressGetPtr) const override;
    void DrawIndexed(const RendererID& inputLayout, uint32_t indexCount, uint32_t primitiveType = 0x0004, uint32_t indexBufferBaseType = 0x1405, uint32_t Offset = 0) const override;
    void DrawIndexedInstanced(const RendererID& inputLayoutID, uint32_t indexCount, uint32_t instanceCount, uint32_t primitiveType = 0x0004, uint32_t indexBufferBaseType = 0x1405, uint32_t Offset = 0) const override;
    void Draw(const RendererID& inputLayoutID, uint32_t vertexCount, uint32_t primitiveType = 0x0004, uint32_t Offset = 0) const override;
    void Clear(FramebufferMask mask) const override;

    void Enable(Capability capability) const override;
    void Enable(std::initializer_list<Capability> capabilities) const override;
    void EnableDebugOutput() const override;
    void Disable(Capability capability) const override;
    void Disable(std::initializer_list<Capability> capabilities) const override;
    void DepthMask(bool enable = true) const override;
    void SetDepthFunc(DepthFunction depthFunc) const override;
    void SetBlendFunction(BlendFunction sfactor, BlendFunction dfactor) const override;
    void SetViewport(int32_t xoffset, int32_t yoffset, uint32_t m_Width, uint32_t m_Height) const override;
    void SetClearColor(const glm::vec4& color) const override;
    void SetClearDepth(const float depth) const override;
    void SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const override;

    RendererID CreateShader(ShaderDescriptor shaderDescriptor) const override;
    RendererID CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) const override;
    void UseShaderProgram(const RendererID& shader) const override;
    void DeleteShader(RendererID& shader) const override;

    //RendererID CreateBuffer(const BufferDescriptor& descriptor) const override;
    //RendererID CreateBuffer(const BufferDescriptor& descriptor, const void* data) const override;
    RendererID CreatePersistenBuffer(const BufferDescriptor descriptor) const override;
    RendererID CreatePersistenBuffer(const BufferDescriptor descriptor, const void* data) const override;
    void* MapBuffer(RendererID rendererId, uint32_t offset, uint32_t size, uint32_t accessFlags) const override;
    void UnmapBuffer(RendererID rendererId) const override;
    //void UploadBufferData(const RendererID& rendererId, const BufferDescriptor& buffer, uint32_t offset, const void* data) const override;
    //void UploadBufferData(const RendererID& rendererId, uint32_t size, uint32_t offset, const void* data) const override;
    void DeleteBuffer(RendererID& rendererId) const override;
    void BindBuffer(BufferBindingTarget bindingTarget, const RendererID& buffer, const uint32_t& bindingSlot) const override;

    DeviceMemoryFence FenceSync() const override;
    void DeleteSync(DeviceMemoryFence gSync) const override;
    FenceSignal ClientWaitSync(DeviceMemoryFence gSync, uint64_t timeout) const override;

    RendererID CreateAttributeLayout(const AttributeLayoutDescriptor& attributeDescriptor) const override;
    RendererID CreateAttributeLayout(RendererID indexBuffer, const AttributeLayoutDescriptor& attributeDescriptor) const override;
    void DeleteInputLayout(RendererID& id) const override;

    RendererID CreateSampler(const SamplerDescriptor& samplerDescriptor) const override;
    RendererID CreateSampler(const std::string& path, SamplerDescriptor& samplerDescriptor) const override;
    void UploadSamplerData(const Sampler& sampler, const Image& image, const uint32_t layer = 0) const override;
    void BindSampler(const RendererID& rendererId, uint32_t unit) const override;
    void DeleteSampler(RendererID& id) const override;
    
    RendererID CreateAttachment(const TextureDescriptorBase& descriptor) const override;
    void DeleteAttachment(RendererID& id) const override;
    RendererID CreateFramebuffer() const override;
    void BindAttachment(const RendererID& framebufferId, const RendererID& attachmentId, const AttachmentPoint& attachmentPoint) const override;
    void CheckFramebufferCompleteness(const RendererID& framebufferId) const override;
    void InvalidateFramebuffer(const RendererID framebufferId, uint32_t invalidAttachmentCount, const AttachmentPoint* attachmentPointArrayData) const override;
    void DeleteFramebuffer(RendererID& rendererId) const override;
    void BindFramebuffer(const RendererID& rendererId) const override;
    void UnbindFramebuffer() const override;
    void BlitFramebuffer(
      const RendererID& sourceId, uint32_t sourceWidth, uint32_t sourceHeight,
      const RendererID& destId, uint32_t destWidth, uint32_t destHeight,
      TextureFilter filter = FILTER_NEAREST, FramebufferMask mask = COLOR_BUFFER) const override;

    //void SetWindowPtr(const Ref<Window>& window) override;
  private:
    //Ref<GLFWwindow> m_GLFWwindow;
  };

}

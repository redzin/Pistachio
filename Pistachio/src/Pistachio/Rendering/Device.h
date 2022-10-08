#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "RenderAPI.h"

namespace Pistachio
{
  class Framebuffer;
  class Window;
  class Device
  {
    friend class RenderPass;
    friend class RenderGraph;

    friend class Buffer;
    friend class Buffer;
    friend class Sampler;
    friend class Attachment;

    friend class Shader;
    friend class Framebuffer;
    friend class AttributeLayout;

  public:
    Device(Window& window);
    ~Device();

    // Create buffers, samplers and attachments.
    Ref<Attachment> CreateAttachment(const AttachmentDescriptor& attachmentDesc);
    Ref<Buffer> CreateBuffer(const BufferDescriptor descriptor, const void* data = nullptr);
    Ref<Sampler> CreateSampler(SamplerDescriptor& samplerDesc);
    Ref<Sampler> CreateOrReplaceSamplerFromFile(const std::string& path, SamplerDescriptor& samplerDesc);
    void UploadSamplerData(const Ref<Sampler> sampler, const Image& image, uint32_t layer = 0);
    
    void SetViewport(int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height);
    Ref<AttributeLayout> RequestAttributeLayout(const AttributeLayoutDescriptor& attributeDescriptor, RendererID indexBuffer = 0);
    Ref<Shader> RequestShader(const std::string& path);

  private:
    Window& m_Window;
    Ref<RenderingAPI> m_RenderingAPI;

    // Shared objects owned by the device are requestable
    Ref<Shader> RequestShader(const std::string& fragmentSrc, const std::string& vertexSrc);
    Ref<Framebuffer> RequestFramebuffer(std::vector<Ref<Attachment>> attachments);
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    std::unordered_map<Hash, Ref<Framebuffer>> m_Framebuffers; // Todo: Convert to TemporaryHashmap that auto-deletes after a few (8-ish) frames if unused
    std::unordered_map<Hash, Ref<AttributeLayout>> m_InputLayouts; // Todo: Convert to TemporaryHashmap that auto-deletes after a few (8-ish) frames if unused

    // These should be called from the individual ptr handle object desctructors (e.g. ~Buffer should call DeleteBuffer -> delete buffer removes it from the pool)
    void DeleteShader(Shader& shader);
    void DeleteBuffer(Buffer& bufferPtr);
    void DeleteSampler(Sampler& sampler);
    void DeleteAttachment(Attachment& attachment);
    void DeleteFramebuffer(Framebuffer& framebuffer);
    void DeleteInputLayout(AttributeLayout& inputLayout);

  };

  class Framebuffer
  {
  public:
    static constexpr uint32_t MAX_COLOR_ATTACHMENTS = 32;
    Framebuffer() = default;
    Framebuffer(Ref<RenderingAPI> renderingAPI, std::vector<Ref<Attachment>> attachments) : m_RenderingAPI(renderingAPI), m_Attachments(attachments)
    {
      m_FramebufferId = m_RenderingAPI->CreateFramebuffer();

      int index = 0;
      for (auto const& attachment : attachments)
      {
        m_AttachmentPointMap[attachment->AttachmentDescriptor.AttachmentPoint] = index;
        m_RenderingAPI->BindAttachment(m_FramebufferId, attachment->RendererID, attachment->AttachmentDescriptor.AttachmentPoint);
        index++;
      }

      m_RenderingAPI->CheckFramebufferCompleteness(m_FramebufferId);
    }

    ~Framebuffer()
    {
      m_RenderingAPI->DeleteFramebuffer(m_FramebufferId);
    }

    //Ref<Attachment> GetAttachment(AttachmentPoint attachmentPoint)
    //{
    //  return m_Attachments[m_AttachmentPointMap[attachmentPoint]];
    //}

  public:
    operator RendererID () const { return m_FramebufferId; }
    operator RendererID& () { return m_FramebufferId; }
    operator const RendererID& () const { return m_FramebufferId; }

  private:
    Ref<RenderingAPI> m_RenderingAPI;
    RendererID m_FramebufferId;
    std::unordered_map<AttachmentPoint, uint32_t> m_AttachmentPointMap;
    std::vector<Ref<Attachment>> m_Attachments;
  };
}




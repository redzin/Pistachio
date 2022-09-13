#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "RenderAPI.h"

namespace Pistacio
{
  class Framebuffer;
  class Window;
  class Device
  {
    friend class RenderPass;
    friend class RenderGraph;

  public:
    Device(Window& window);
    ~Device();

    // Todo: implement compute passes
    // Todo: persistent buffers with barriers
    void CreateBuffer(const std::string& name, const BufferDescriptor& descriptor, const void* data = nullptr);
    void UploadBufferData(const Buffer& buffer, const void* data, uint32_t offset = 0);
    void CreatePersistentBuffer(const std::string& name, const PersistentBufferDescriptor descriptor, const void* data = nullptr);
    void RefreshBufferFence(const std::string& name);
    void WaitForBufferFence(const std::string& name);


    void CreateSampler(const std::string name, TextureDescriptorBase& textureDesc, SamplerDescriptor& samplerDesc);
    void CreateOrReplaceSamplerFromFile(const std::string& path, TextureDescriptorBase& textureDesc, SamplerDescriptor& samplerDesc);
    void UploadSamplerData(const std::string& name, const Image& image, uint32_t layer = 0);

    Attachment* CreateAttachment(const std::string& semanticName, const TextureDescriptorBase& textureDesc, const AttachmentDescriptor& attachmentDesc);
    
    Framebuffer* RequestFramebuffer(std::vector<Attachment*> attachments);
    Sampler* RequestSampler(const std::string& semanticName);
    Attachment* RequestAttachment(const std::string& semanticName);
    Attachment* ReplaceAttachment(const std::string& semanticName, const TextureDescriptorBase textureDesc, const AttachmentDescriptor attachmentDesc);
    Buffer* RequestBuffer(const std::string& semanticName);
    PersistentBuffer* RequestPersistentBuffer(const std::string& semanticName);
    Shader* RequestShader(const std::string& path);
    Shader* RequestShader(const std::string& fragmentSrc, const std::string& vertexSrc);
    InputLayout* RequestInputLayout(std::vector<std::tuple<Buffer*, BufferMemoryLayout>> vertexBuffers, RendererID indexBuffer = 0);
    InputLayout* RequestInputLayout(std::vector<std::tuple<Buffer*, BufferMemoryLayout>> vertexBuffers, std::vector<std::tuple<PersistentBuffer*, BufferMemoryLayout>> vertexPersistentBuffers, RendererID indexBuffer = 0);
    void SetViewport(int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height);

  private:
    Window& m_Window;
    Ref<RenderingAPI> m_RenderingAPI;

    // Todo: Write proper reference counting system so we know when to delete pooled objects
    std::unordered_map<std::string, Shader*> m_Shaders;
    std::unordered_map<std::string, Buffer*> m_Buffers;
    std::unordered_map<std::string, PersistentBuffer*> m_PersistentBuffers;
    std::unordered_map<std::string, Sampler*> m_Samplers;
    std::unordered_map<std::string, Attachment*> m_Attachments;
    std::unordered_map<Hash, Framebuffer*> m_Framebuffers;
    std::unordered_map<Hash, InputLayout*> m_InputLayouts;

    ObjectPool<Shader> m_ShaderPool;
    ObjectPool<Buffer> m_BufferPool;
    ObjectPool<PersistentBuffer> m_PersistentBufferPool;
    ObjectPool<Sampler> m_SamplerPool;
    ObjectPool<Attachment> m_AttachmentPool;
    ObjectPool<Framebuffer> m_FramebufferPool; // Todo: Convert to TemporaryHashmap that auto-deletes after a few (8-ish) frames if unused
    ObjectPool<InputLayout> m_InputLayoutPool; // Todo: Convert to TemporaryHashmap that auto-deletes after a few (8-ish) frames if unused


    // These should be called from the individual ptr handle object desctructors (e.g. ~Buffer should call DeleteBuffer -> delete buffer removes it from the pool)
    void DeleteShader(Shader* shader);
    void DeleteBuffer(Buffer* bufferPtr);
    void DeletePersistentBuffer(PersistentBuffer* bufferPtr);
    void DeleteSampler(Sampler* sampler);
    void DeleteAttachment(Attachment* attachment);
    void DeleteFramebuffer(Framebuffer* framebuffer);
    void DeleteInputLayout(InputLayout* inputLayout);

  };

  class Framebuffer
  {
  public:
    static constexpr uint32_t MAX_COLOR_ATTACHMENTS = 32;
    Framebuffer(Ref<RenderingAPI> renderingAPI, std::vector<Attachment*> attachments) : m_RenderingAPI(renderingAPI), m_Attachments(attachments)
    {
      m_FramebufferId = m_RenderingAPI->CreateFramebuffer();

      int index = 0;
      for (auto const& attachment : attachments)
      {
        m_AttachmentPointMap[attachment->AttachmentDescriptor.AttachmentPoint] = index;
        m_RenderingAPI->CreateAttachment(m_FramebufferId, attachment->RendererID, attachment->AttachmentDescriptor.AttachmentPoint);
        index++;
      }

      m_RenderingAPI->CheckFramebufferCompleteness(m_FramebufferId);
    }

    ~Framebuffer()
    {
      m_RenderingAPI->DeleteFramebuffer(m_FramebufferId);
    }

    Attachment* GetAttachment(AttachmentPoint attachmentPoint)
    {
      return m_Attachments[m_AttachmentPointMap[attachmentPoint]];
    }

  public:
    operator RendererID () const { return m_FramebufferId; }
    operator RendererID& () { return m_FramebufferId; }
    operator const RendererID& () const { return m_FramebufferId; }

  private:
    Ref<RenderingAPI> m_RenderingAPI;
    RendererID m_FramebufferId;
    std::unordered_map<AttachmentPoint, uint32_t> m_AttachmentPointMap;
    std::vector<Attachment*> m_Attachments;
  };
}




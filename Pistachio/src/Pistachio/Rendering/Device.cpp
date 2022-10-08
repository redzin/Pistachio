#include "pch.h"
#include "Device.h"

namespace Pistachio
{

  Device::Device(Window& window) : m_Window(window)
  {
    m_Window.MakeRenderingContextCurrent();
    m_RenderingAPI = RenderingAPI::Create();
    int LoadStatus = m_RenderingAPI->InitExternalProcAddress(m_Window.GetProcAddress());
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    PSTC_CORE_ASSERT(status, "Failed to to load glad!");
    
    m_RenderingAPI->EnableDebugOutput();

    PSTC_CORE_INFO("OpenGL Info:");
    PSTC_CORE_TRACE("  OpenGL Vendor: {0}", glGetString(GL_VENDOR));
    PSTC_CORE_TRACE("  OpenGL Renderer: {0}", glGetString(GL_RENDERER));
    PSTC_CORE_TRACE("  OpenGL Version: {0}", glGetString(GL_VERSION));
    PSTC_CORE_TRACE("  Shading Language: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
#ifdef PSTC_VERBOSE_DEBUG
    int max = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &max);
    PSTC_CORE_TRACE("  OpenGL Extensions:");
    for (int i = 0; i < max; i++)
      PSTC_CORE_TRACE("    {0}", glGetStringi(GL_EXTENSIONS, i));
#endif
  }

  Device::~Device()
  {
    
  }

  Ref<Attachment> Device::CreateAttachment(const AttachmentDescriptor& attachmentDesc)
  {
    RendererID rendererID = m_RenderingAPI->CreateAttachment(attachmentDesc.TextureDescriptor);
    return CreateRef<Attachment>(*this, rendererID, attachmentDesc);
  }

  Ref<Buffer> Device::CreateBuffer(const BufferDescriptor descriptor, const void* data)
  {
    RendererID rendererID = m_RenderingAPI->CreatePersistenBuffer(descriptor, data);
    void* mapPtr = m_RenderingAPI->MapBuffer(rendererID, 0, descriptor.Size, descriptor.Flags);
    return CreateRef<Buffer>(*this, rendererID, descriptor, mapPtr);
  }

  Ref<Sampler> Device::CreateOrReplaceSamplerFromFile(const std::string& path, SamplerDescriptor& samplerDesc)
  {
    RendererID rendererID = m_RenderingAPI->CreateSampler(path, samplerDesc);
    return CreateRef<Sampler>(*this, rendererID, samplerDesc);
  }

  Ref<Sampler> Device::CreateSampler(SamplerDescriptor& samplerDesc)
  {
    RendererID rendererID = m_RenderingAPI->CreateSampler(samplerDesc);
    return CreateRef<Sampler>(*this, rendererID, samplerDesc);
  }

  void Device::UploadSamplerData(const Ref<Sampler> sampler, const Image& image, uint32_t layer)
  {
    m_RenderingAPI->UploadSamplerData(*sampler, image, layer);
  }

  Ref<Shader> Device::RequestShader(const std::string& path)
  {
    if (m_Shaders.find(path) != m_Shaders.end())
      return m_Shaders[path];

    auto rendererId = m_RenderingAPI->CreateShader(path);
    m_Shaders[path] = CreateRef<Shader>(rendererId);
    return m_Shaders[path];
  }

  Ref<Shader> Device::RequestShader(const std::string& vertexSrc, const std::string& fragmentSrc)
  {
    std::string hash = "";
    hash += static_cast<uint64_t>(std::hash<std::string>{}(fragmentSrc));
    hash += static_cast<uint64_t>(std::hash<std::string>{}(vertexSrc));

    if (m_Shaders.find(hash) != m_Shaders.end())
      return m_Shaders[hash];

    auto rendererId = m_RenderingAPI->CreateShader(vertexSrc, fragmentSrc);
    m_Shaders[hash] = CreateRef<Shader>(rendererId);
    return m_Shaders[hash];
  }

  Ref<Framebuffer> Device::RequestFramebuffer(std::vector<Ref<Attachment>> attachments)
  {
    Hasher hasher;
    for (auto const& attachment : attachments)
    {
      hasher.hash(attachment->RendererID);
      hasher.hash(attachment->AttachmentDescriptor.TextureDescriptor.InternalFormat);
      hasher.hash(attachment->AttachmentDescriptor.TextureDescriptor.Layers);
      hasher.hash(attachment->AttachmentDescriptor.TextureDescriptor.Samples);
      hasher.hash(attachment->AttachmentDescriptor.TextureDescriptor.SizeX);
      hasher.hash(attachment->AttachmentDescriptor.TextureDescriptor.SizeY);
      hasher.hash(attachment->AttachmentDescriptor.TextureDescriptor.SizeZ);
      hasher.hash(attachment->AttachmentDescriptor.TextureDescriptor.Type);
      hasher.hash(attachment->AttachmentDescriptor.AttachmentPoint);
    }
    Hash hash = hasher.get();

    if (m_Framebuffers.find(hash) != m_Framebuffers.end())
      return m_Framebuffers[hash];

    m_Framebuffers[hash] = CreateRef<Framebuffer>(m_RenderingAPI, attachments);
    return m_Framebuffers[hash];
  }

  Ref<AttributeLayout> Device::RequestAttributeLayout(const AttributeLayoutDescriptor& attributeDescriptor, RendererID indexBuffer)
  {
    Hasher hasher;
    hasher.hash(indexBuffer);
    for (const auto& attribute : attributeDescriptor)
    {
      hasher.hash(attribute.Buffer->RendererID);
      hasher.hash(attribute.Buffer->Descriptor.AttributeDivisor);
      hasher.hash(attribute.Buffer->Descriptor.Size);
      hasher.hash(attribute.Buffer->Descriptor.Flags);
      for (auto& entry : attribute.MemoryLayout)
      {
        hasher.data(entry.SemanticName.c_str(), entry.SemanticName.size());
        hasher.hash(static_cast<uint64_t>(entry.Normalized));
        hasher.hash(static_cast<uint64_t>(entry.Offset));
        hasher.hash(static_cast<uint64_t>(entry.Size));
        hasher.hash(static_cast<uint64_t>(entry.Type));
      }
    }
    Hash hash = hasher.get();

    if (m_InputLayouts.find(hash) != m_InputLayouts.end())
      return m_InputLayouts[hash];

    RendererID rendererID = m_RenderingAPI->CreateAttributeLayout(indexBuffer, attributeDescriptor);
    m_InputLayouts[hash] = CreateRef<AttributeLayout>(rendererID, attributeDescriptor);
    return m_InputLayouts[hash];
  }

  void Device::SetViewport(int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height)
  {
    m_RenderingAPI->SetViewport(xoffset, yoffset, width, height);
  }

  void Device::DeleteShader(Shader& shader)
  {
    m_RenderingAPI->DeleteShader(shader.RendererID);
  }

  void Device::DeleteBuffer(Buffer& bufferPtr)
  {
    m_RenderingAPI->UnmapBuffer(bufferPtr.RendererID);
    m_RenderingAPI->DeleteBuffer(bufferPtr.RendererID);
  }

  void Device::DeleteSampler(Sampler& sampler)
  {
    m_RenderingAPI->DeleteSampler(sampler.RendererID);
  }

  void Device::DeleteAttachment(Attachment& attachment)
  {
    m_RenderingAPI->DeleteAttachment(attachment.RendererID);
  }

  void Device::DeleteFramebuffer(Framebuffer& framebuffer)
  {
    m_RenderingAPI->DeleteFramebuffer(framebuffer);
  }

  void Device::DeleteInputLayout(AttributeLayout& inputLayout)
  {
    m_RenderingAPI->DeleteInputLayout(inputLayout.RendererID);
  }

}


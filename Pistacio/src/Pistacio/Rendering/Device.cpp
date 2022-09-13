#include "pch.h"
#include "Device.h"

namespace Pistacio
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
    for (auto const [SemanticName, shader] : m_Shaders)
    {
      DeleteShader(shader);
    }
    for (auto const [SemanticName, buffer] : m_Buffers)
    {
      DeleteBuffer(buffer);
    }
    for (auto const [SemanticName, buffer] : m_PersistentBuffers)
    {
      DeletePersistentBuffer(buffer);
    }
    for (auto const [SemanticName, sampler] : m_Samplers)
    {
      DeleteSampler(sampler);
    }
    for (auto const [SemanticName, attachment] : m_Attachments)
    {
      DeleteAttachment(attachment);
    }
    for (auto const [SemanticName, framebuffer] : m_Framebuffers)
    {
      DeleteFramebuffer(framebuffer);
    }
  }

  void Device::CreateBuffer(const std::string& SemanticName, const BufferDescriptor& descriptor, const void* data)
  {
    PSTC_CORE_ASSERT(m_Buffers.find(SemanticName) == m_Buffers.end(), "Buffer semantic name already exists!");

    RendererID rendererID = m_RenderingAPI->CreateBuffer(descriptor, data);
    Buffer* bufferPtr = m_BufferPool.Allocate(rendererID, descriptor);
    m_Buffers[SemanticName] = bufferPtr;
  }

  void Device::UploadBufferData(const Buffer& buffer, const void* data, uint32_t Offset)
  {
    m_RenderingAPI->UploadBufferData(buffer.RendererID, buffer.Descriptor, Offset, data);
  }

  void Device::CreatePersistentBuffer(const std::string& name, const PersistentBufferDescriptor descriptor, const void* data)
  {
    PSTC_CORE_ASSERT(m_PersistentBuffers.find(name) == m_PersistentBuffers.end(), "Persistent buffer semantic name already exists!");

    RendererID rendererID = m_RenderingAPI->CreatePersistenBuffer(descriptor, data);
    void* mapPtr = m_RenderingAPI->MapPersistentBuffer(rendererID, 0, descriptor.Size, descriptor.Flags);
    PersistentBuffer* bufferPtr = m_PersistentBufferPool.Allocate(rendererID, descriptor, mapPtr);
    m_PersistentBuffers[name] = bufferPtr;
  }

  void Device::RefreshBufferFence(const std::string& name)
  {

  }

  void Device::WaitForBufferFence(const std::string& name)
  {
  }

  void Device::CreateOrReplaceSamplerFromFile(const std::string& path, TextureDescriptorBase& textureDesc, SamplerDescriptor& samplerDesc)
  {
    RendererID rendererID = m_RenderingAPI->CreateSampler(path, samplerDesc, textureDesc);
    m_Samplers[path] = m_SamplerPool.Allocate(rendererID, samplerDesc, textureDesc);
  }

  void Device::CreateSampler(const std::string name, TextureDescriptorBase& textureDesc, SamplerDescriptor& samplerDesc)
  {
    RendererID rendererID = m_RenderingAPI->CreateSampler(textureDesc, samplerDesc);
    m_Samplers[name] = m_SamplerPool.Allocate(rendererID, samplerDesc, textureDesc);
  }

  void Device::UploadSamplerData(const std::string& name, const Image& image, uint32_t layer)
  {
    Sampler* sampler = m_Samplers[name];
    if (!sampler)
      return;

    m_RenderingAPI->UploadSamplerData(*sampler, image, layer);
  }

  Shader* Device::RequestShader(const std::string& path)
  {
    //auto name = remove_extension(base_name(path));
    if (m_Shaders.find(path) != m_Shaders.end()) return m_Shaders[path];

    auto rendererId = m_RenderingAPI->CreateShader(path);
    Shader* ptr = m_ShaderPool.Allocate(rendererId);
    m_Shaders[path] = ptr;
    return ptr;
  }

  Shader* Device::RequestShader(const std::string& vertexSrc, const std::string& fragmentSrc)
  {
    std::string hash = "";
    hash += static_cast<uint64_t>(std::hash<std::string>{}(fragmentSrc));
    hash += static_cast<uint64_t>(std::hash<std::string>{}(vertexSrc));

    if (m_Shaders[hash]) return m_Shaders[hash];

    auto rendererId = m_RenderingAPI->CreateShader(vertexSrc, fragmentSrc);
    return m_ShaderPool.Allocate(rendererId);
  }

  Sampler* Device::RequestSampler(const std::string& semanticName)
  {
    if (m_Samplers[semanticName])
      return m_Samplers[semanticName];
    
    Attachment* attachment = m_Attachments[semanticName];
    if (!attachment)
      return nullptr;

    TextureDescriptorBase textureDesc;
    textureDesc = attachment->TextureBase;

    SamplerDescriptor samplerDesc;
    samplerDesc.MagFilter = FILTER_LINEAR_MIPMAP_NEAREST;
    samplerDesc.MinFilter = FILTER_LINEAR_MIPMAP_NEAREST;
    samplerDesc.MipLevels = 1;
    
    Sampler* ptr = m_SamplerPool.Allocate(attachment->RendererID, samplerDesc, textureDesc);
    m_Samplers[semanticName] = ptr;
    return ptr;
  }

  Attachment* Device::RequestAttachment(const std::string& semanticName)
  {
    return m_Attachments[semanticName];
  }

  Buffer* Device::RequestBuffer(const std::string& semanticName)
  {
    return m_Buffers[semanticName];
  }

  PersistentBuffer* Device::RequestPersistentBuffer(const std::string& semanticName)
  {
    return m_PersistentBuffers[semanticName];
  }

  Attachment* Device::CreateAttachment(const std::string& semanticName, const TextureDescriptorBase& textureDesc, const AttachmentDescriptor& attachmentDesc)
  {
    RendererID rendererID = m_RenderingAPI->CreateAttachment(textureDesc);
    Attachment* ptr = m_AttachmentPool.Allocate(rendererID, textureDesc, attachmentDesc);
    m_Attachments[semanticName] = ptr;
    return ptr;
  }

  Attachment* Device::ReplaceAttachment(const std::string& semanticName, const TextureDescriptorBase textureDesc, const AttachmentDescriptor attachmentDesc)
  {
    DeleteAttachment(RequestAttachment(semanticName));
    return CreateAttachment(semanticName, textureDesc, attachmentDesc);
  }

  Framebuffer* Device::RequestFramebuffer(std::vector<Attachment*> attachments)
  {
    Hasher hasher;
    for (auto const& attachment : attachments)
    {
      hasher.hash(attachment->RendererID);
      hasher.hash(attachment->TextureBase.InternalFormat);
      hasher.hash(attachment->TextureBase.Layers);
      hasher.hash(attachment->TextureBase.Samples);
      hasher.hash(attachment->TextureBase.SizeX);
      hasher.hash(attachment->TextureBase.SizeY);
      hasher.hash(attachment->TextureBase.SizeZ);
      hasher.hash(attachment->TextureBase.Type);
      hasher.hash(attachment->AttachmentDescriptor.AttachmentPoint);
    }
    Hash hash = hasher.get();
    
    if (m_Framebuffers[hash]) return m_Framebuffers[hash];

    Framebuffer* ptr = m_FramebufferPool.Allocate(m_RenderingAPI, attachments);
    m_Framebuffers[hash] = ptr;
    return ptr;
  }

  InputLayout* Device::RequestInputLayout(std::vector<std::tuple<Buffer*, BufferMemoryLayout>> vertexBuffers, std::vector<std::tuple<PersistentBuffer*, BufferMemoryLayout>> vertexPersistentBuffers, RendererID indexBuffer)
  {
    Hasher hasher;
    hasher.hash(indexBuffer);
    for (auto& [buffer, layout] : vertexBuffers)
    {
      hasher.hash(buffer->RendererID);
      hasher.hash(buffer->Descriptor.AttributeDivisor);
      hasher.hash(buffer->Descriptor.Size);
      hasher.hash(buffer->Descriptor.Usage);
      for (auto& entry : layout)
      {
        hasher.data(entry.SemanticName.c_str(), entry.SemanticName.size());
        hasher.hash(static_cast<uint64_t>(entry.Normalized));
        hasher.hash(static_cast<uint64_t>(entry.Offset));
        hasher.hash(static_cast<uint64_t>(entry.Size));
        hasher.hash(static_cast<uint64_t>(entry.Type));
      }
    }
    for (auto& [buffer, layout] : vertexPersistentBuffers)
    {
      hasher.hash(buffer->RendererID);
      hasher.hash(buffer->Descriptor.AttributeDivisor);
      hasher.hash(buffer->Descriptor.Size);
      hasher.hash(buffer->Descriptor.Flags);
      for (auto& entry : layout)
      {
        hasher.data(entry.SemanticName.c_str(), entry.SemanticName.size());
        hasher.hash(static_cast<uint64_t>(entry.Normalized));
        hasher.hash(static_cast<uint64_t>(entry.Offset));
        hasher.hash(static_cast<uint64_t>(entry.Size));
        hasher.hash(static_cast<uint64_t>(entry.Type));
      }
    }
    Hash hash = hasher.get();

    if (m_InputLayouts[hash]) return m_InputLayouts[hash];

    RendererID rendererID = m_RenderingAPI->CreateInputLayout(indexBuffer, vertexBuffers, vertexPersistentBuffers);
    InputLayout* ptr = m_InputLayoutPool.Allocate(rendererID);
    m_InputLayouts[hash] = ptr;
    return ptr;
  }

  InputLayout* Device::RequestInputLayout(std::vector<std::tuple<Buffer*, BufferMemoryLayout>> vertexBuffers, RendererID indexBuffer)
  {
    Hasher hasher;
    hasher.hash(indexBuffer);
    for (auto& [buffer, layout] : vertexBuffers)
    {
      hasher.hash(buffer->RendererID);
      hasher.hash(buffer->Descriptor.AttributeDivisor);
      hasher.hash(buffer->Descriptor.Size);
      hasher.hash(buffer->Descriptor.Usage);
      for (auto& entry : layout)
      {
        hasher.data(entry.SemanticName.c_str(), entry.SemanticName.size());
        hasher.hash(static_cast<uint64_t>(entry.Normalized));
        hasher.hash(static_cast<uint64_t>(entry.Offset));
        hasher.hash(static_cast<uint64_t>(entry.Size));
        hasher.hash(static_cast<uint64_t>(entry.Type));
      }
    }
    Hash hash = hasher.get();

    if (m_InputLayouts[hash]) return m_InputLayouts[hash];

    RendererID rendererID = m_RenderingAPI->CreateInputLayout(indexBuffer, vertexBuffers);
    InputLayout* ptr = m_InputLayoutPool.Allocate(rendererID);
    m_InputLayouts[hash] = ptr;
    return ptr;
  }

  void Device::SetViewport(int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height)
  {
    m_RenderingAPI->SetViewport(xoffset, yoffset, width, height);
  }

  void Device::DeleteShader(Shader* shader)
  {
    if (!shader) return;
    m_RenderingAPI->DeleteShader(shader->RendererID);
    m_ShaderPool.Free(shader);
  }

  void Device::DeleteBuffer(Buffer* bufferPtr)
  {
    if (!bufferPtr) return;
    m_RenderingAPI->DeleteBuffer(bufferPtr->RendererID);
    m_BufferPool.Free(bufferPtr);
  }

  void Device::DeletePersistentBuffer(PersistentBuffer* bufferPtr)
  {
    if (!bufferPtr) return;
    m_RenderingAPI->UnmapBuffer(bufferPtr->RendererID);
    m_RenderingAPI->DeleteBuffer(bufferPtr->RendererID);
    m_PersistentBufferPool.Free(bufferPtr);
  }

  void Device::DeleteSampler(Sampler* sampler)
  {
    if (!sampler) return;
    m_RenderingAPI->DeleteSampler(sampler->RendererID);
    m_SamplerPool.Free(sampler);
  }

  void Device::DeleteAttachment(Attachment* attachment)
  {
    if (!attachment) return;
    m_RenderingAPI->DeleteAttachment(attachment->RendererID);
    m_AttachmentPool.Free(attachment);
  }

  void Device::DeleteFramebuffer(Framebuffer* framebuffer)
  {
    if (!framebuffer) return;
    m_RenderingAPI->DeleteFramebuffer(*framebuffer);
    m_FramebufferPool.Free(framebuffer);
  }

  void Device::DeleteInputLayout(InputLayout* inputLayout)
  {
    if (!inputLayout) return;
    m_RenderingAPI->DeleteInputLayout(inputLayout->RendererID);
    m_InputLayoutPool.Free(inputLayout);
  }

}


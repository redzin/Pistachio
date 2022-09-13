#include "pch.h"
#include "RenderGraph.h"


namespace Pistacio
{

  /////////////////////////////////////////////////////////
  /// RenderGraph
  /////////////////////////////////////////////////////////

  RenderPass* RenderGraph::AddPass(const std::string& semanticName)
  {
    PSTC_CORE_ASSERT(m_RenderPasses.find(semanticName) == m_RenderPasses.end(), "Render pass semantic name already taken!");

    RenderPass* passPtr = m_RenderPassPool.Allocate(*this, semanticName);
    m_RenderPasses[semanticName] = passPtr;
    return m_RenderPasses[semanticName];
  }

  void RenderGraph::Cook(Device& device)
  {
    if (m_IsCooked)
      return;

    m_IsCooked = true;

    for (auto& [SemanticName, pass] : m_RenderPasses) // temporary...
      m_CookedRenderPasses.push_back(pass);

    // make sure physical resources exists
    // compute render pass dependencies
    // add to m_CookedRenderPasses in order of dependency

  }

  static const std::set<Capability> SetOfAllCapabilities
  {
    Capability::CullFace,
    Capability::DepthTest,
    Capability::ScissorTest,
    Capability::StencilTest,
    Capability::Blend
  };

  static void ResizeAttachment(Device& device, RenderResource* attachmentResource, uint32_t width, uint32_t height)
  {
    Attachment* attachment = device.RequestAttachment(attachmentResource->Name);
    if (attachment && (attachment->TextureBase.SizeX != width || attachment->TextureBase.SizeY != height))
    {
      attachment->TextureBase.SizeX = width;
      attachment->TextureBase.SizeY = height;
      device.ReplaceAttachment(attachmentResource->Name, attachment->TextureBase, attachment->AttachmentDescriptor);
    }
  }

  void RenderGraph::Render(Device& device, Window& window, uint32_t viewport_width, uint32_t viewport_height)
  {
    Cook(device); // should be removed and the client should manage when to cook. No need to recook if no new passes are added, and the client should know

    for (const auto& pass : m_CookedRenderPasses)
    {
      std::vector<Attachment*> attachments;
      if (pass->m_DepthStencilOutput)
      {
        ResizeAttachment(device, pass->m_DepthStencilOutput, viewport_width, viewport_height);
        attachments.push_back(device.RequestAttachment(pass->m_DepthStencilOutput->Name));
      }
      for (const auto& colorAttachment : pass->m_ColorAttachmentOuputs)
      {
        ResizeAttachment(device, colorAttachment, viewport_width, viewport_height);
        attachments.push_back(device.RequestAttachment(colorAttachment->Name));
      }
      Framebuffer* framebuffer = device.RequestFramebuffer(attachments);
      
      std::vector<Capability> disabledCapabilities;
      std::set_symmetric_difference(
        pass->m_RenderState.Capabilities.begin(), pass->m_RenderState.Capabilities.end(),
        SetOfAllCapabilities.begin(), SetOfAllCapabilities.end(),
        std::back_inserter(disabledCapabilities));
      
      for (const auto& cap : disabledCapabilities)
        device.m_RenderingAPI->Disable(cap);
      
      for (const auto& cap : pass->m_RenderState.Capabilities)
        device.m_RenderingAPI->Enable(cap);
      
      device.m_RenderingAPI->DepthMask(pass->m_RenderState.DepthMask);
      device.m_RenderingAPI->SetDepthFunc(pass->m_RenderState.DepthFunction);
      device.m_RenderingAPI->SetBlendFunction(pass->m_RenderState.BlendState.SourceBlendFunction, pass->m_RenderState.BlendState.DestinationBlendFunction);

      //std::vector<std::tuple<Buffer*, BufferMemoryLayout>> vertexBuffers; // todo: make generalized buffer layout descriptor
      //for (const auto& vertexBufferResource : pass->m_VertexBufferInputs)
      //{
      //  Buffer* vertexBuffer = device.RequestBuffer(vertexBufferResource->Name);
      //  vertexBuffers.push_back(std::tuple(vertexBuffer, vertexBufferResource->Layout));
      //}
      //RendererID indexBuffer = 0;
      //if (pass->m_IndexBufferInput)
      //  indexBuffer = device.RequestBuffer(pass->m_IndexBufferInput->Name)->RendererID;
      //InputLayout* inputLayout = device.RequestInputLayout(vertexBuffers, indexBuffer);
      
      std::vector<std::tuple<Buffer*, BufferMemoryLayout>> vertexBuffers;
      std::vector<std::tuple<PersistentBuffer*, BufferMemoryLayout>> vertexPersistentBuffers;
      for (const auto& vertexBufferResource : pass->m_VertexBufferInputs)
      {
        Buffer* vertexBuffer = device.RequestBuffer(vertexBufferResource->Name);
        if (vertexBuffer)
        {
          vertexBuffers.push_back(std::tuple(vertexBuffer, vertexBufferResource->Layout));
          continue;
        }
        PersistentBuffer* vertexPersistentBuffer = device.RequestPersistentBuffer(vertexBufferResource->Name);
        if (vertexPersistentBuffer)
        {
          vertexPersistentBuffers.push_back(std::tuple(vertexPersistentBuffer, vertexBufferResource->Layout));
          continue;
        }
      }
      RendererID indexBuffer = 0;
      if (pass->m_IndexBufferInput)
        indexBuffer = device.RequestBuffer(pass->m_IndexBufferInput->Name)->RendererID;
      InputLayout* inputLayout = device.RequestInputLayout(vertexBuffers, vertexPersistentBuffers, indexBuffer);


      device.m_RenderingAPI->SetViewport(0, 0, viewport_width, viewport_height);
      device.m_RenderingAPI->UseShaderProgram(device.RequestShader(pass->m_ShaderResource->Name)->RendererID);
      device.m_RenderingAPI->BindFramebuffer(*framebuffer);
      pass->m_CommandBuffer(*device.m_RenderingAPI, inputLayout->RendererID);

      if (pass->m_ResolveOutputs.size() > 0 && pass->m_ColorAttachmentOuputs.size() > 0)
      {
        std::vector<Attachment*> resolveAttachments;
        for (const auto& resolveAttachment : pass->m_ResolveOutputs)
        {
          ResizeAttachment(device, resolveAttachment, viewport_width, viewport_height);
          resolveAttachments.push_back(device.RequestAttachment(resolveAttachment->Name));
        }
        Framebuffer* resolveFramebuffer = device.RequestFramebuffer(resolveAttachments);

        device.m_RenderingAPI->Disable(Capability::StencilTest);
        device.m_RenderingAPI->Disable(Capability::DepthTest);
        device.m_RenderingAPI->Disable(Capability::ScissorTest);
        device.m_RenderingAPI->BlitFramebuffer
        (
          *framebuffer, viewport_width, viewport_height,
          *resolveFramebuffer, viewport_width, viewport_height,
          FILTER_NEAREST, COLOR_BUFFER | DEPTH_BUFFER
        );
      }

      device.m_RenderingAPI->UnbindFramebuffer();

    }

  }

  RenderShaderResource* RenderGraph::RequestShaderResource(const std::string& SemanticName)
  {
    if (m_ShaderResources[SemanticName])
      return m_ShaderResources[SemanticName];

    return m_ShaderResourcePool.Allocate(SemanticName);
  }

  RenderResource* RenderGraph::RequestResource(const std::string& SemanticName)
  {
    if (m_RenderResources[SemanticName])
      return m_RenderResources[SemanticName];

    return m_RenderResourcePool.Allocate(SemanticName);
  }

  VertexBufferResource* RenderGraph::RequestVertexBufferResource(const std::string& SemanticName)
  {
    if (m_VertexBufferResources[SemanticName])
      return m_VertexBufferResources[SemanticName];

    return m_VertexBufferResourcePool.Allocate(SemanticName);
  }

  /////////////////////////////////////////////////////////
  /// RenderPassState
  /////////////////////////////////////////////////////////

  std::set<Capability> GetCapabilitySet(std::initializer_list<Capability>&& capabilities)
  {
    std::set<Capability> set;
    for (const auto& cap : capabilities)
      set.insert(cap);
    return set;
  }

  RenderPassState::RenderPassState(std::initializer_list<Capability> capabilities, Pistacio::DepthFunction depthFunction, Pistacio::BlendState blendState, bool depthMask)
    : Capabilities(GetCapabilitySet(std::move(capabilities))), DepthFunction(depthFunction), BlendState(blendState), DepthMask(depthMask)
  {
  }



  /////////////////////////////////////////////////////////
  /// RenderPass
  /////////////////////////////////////////////////////////

  void RenderPass::SetShaderProgram(const std::string&& SemanticName)
  {
    RenderShaderResource* shaderPtr = m_RenderGraph.RequestShaderResource(SemanticName);
    shaderPtr->UsedInPasses.insert(m_Name);
    m_ShaderResource = shaderPtr;
    m_RenderGraph.m_IsCooked = false;
  }

  void RenderPass::SetDepthStencilInput(const std::string& SemanticName)
  {
    RenderResource* attachment = m_RenderGraph.RequestResource(SemanticName);
    attachment->Name = SemanticName;
    attachment->ReadInPasses.insert(m_Name);
    m_DepthStencilInput = attachment;
    m_RenderGraph.m_IsCooked = false;
  }

  void RenderPass::SetDepthStencilOutput(const std::string SemanticName)
  {
    RenderResource* attachment = m_RenderGraph.RequestResource(SemanticName);
    attachment->Name = SemanticName;
    attachment->WrittenInPasses.insert(m_Name);
    m_DepthStencilOutput = attachment;
    m_RenderGraph.m_IsCooked = false;
  }
  
  void RenderPass::AddAttachmentInput(const std::string& SemanticName)
  {
    RenderResource* attachment = m_RenderGraph.RequestResource(SemanticName);
    attachment->Name = SemanticName;
    attachment->ReadInPasses.insert(m_Name);
    m_ColorAttachmentInputs.push_back(attachment);
    m_RenderGraph.m_IsCooked = false;
  }

  void RenderPass::AddAttachmentOutput(const std::string& SemanticName)
  {
    RenderResource* attachment = m_RenderGraph.RequestResource(SemanticName);
    attachment->Name = SemanticName;
    attachment->WrittenInPasses.insert(m_Name);
    m_ColorAttachmentOuputs.push_back(attachment);
    m_RenderGraph.m_IsCooked = false;
  }
  
  void RenderPass::AddSamplerInput(const std::string& SemanticName)
  {
    RenderResource* samplerPtr = m_RenderGraph.RequestResource(SemanticName);
    samplerPtr->Name = SemanticName;
    samplerPtr->ReadInPasses.insert(m_Name);
    m_SamplerInputs.push_back(samplerPtr);
    m_RenderGraph.m_IsCooked = false;
  }
  
  void RenderPass::AddVertexBufferInput(const std::string& SemanticName, const BufferMemoryLayout& vertexLayout)
  {
    VertexBufferResource* bufferPtr = m_RenderGraph.RequestVertexBufferResource(SemanticName);
    bufferPtr->Name = SemanticName;
    bufferPtr->UsedInPasses.insert(m_Name);
    bufferPtr->Layout = vertexLayout;
    m_VertexBufferInputs.push_back(bufferPtr);
    m_RenderGraph.m_IsCooked = false;
  }
  
  void RenderPass::AddIndexBufferInput(const std::string& SemanticName)
  {
    RenderResource* bufferPtr = m_RenderGraph.RequestResource(SemanticName);
    bufferPtr->Name = SemanticName;
    bufferPtr->ReadInPasses.insert(m_Name);
    m_IndexBufferInput = bufferPtr;
    m_RenderGraph.m_IsCooked = false;
  }

  void RenderPass::AddResolveOutput(const std::string& SemanticName)
  {
    RenderResource* resolveOutput = m_RenderGraph.RequestResource(SemanticName);
    resolveOutput->Name = SemanticName;
    resolveOutput->WrittenInPasses.insert(m_Name);
    m_ResolveOutputs.push_back(resolveOutput);
    m_RenderGraph.m_IsCooked = false;
  }

  void RenderPass::SetRenderState(RenderPassState&& state)
  {
    m_RenderState = std::move(state);
  }

  void RenderPass::RecordCommandBuffer(std::function<void(RenderingAPI&, RendererID)> commands)
  {
    m_CommandBuffer = commands;
  }

  void RenderPass::Resize(Device& device, uint32_t width, uint32_t height)
  {
    if (m_DepthStencilOutput)
      ResizeAttachment(device, m_DepthStencilOutput, width, height);
    
    for (const auto& attachmentResource : m_ColorAttachmentOuputs)
      ResizeAttachment(device, attachmentResource, width, height);

    for (const auto& attachmentResource : m_ResolveOutputs)
      ResizeAttachment(device, attachmentResource, width, height);

  }



  //void RenderGraph::Init(uint32_t m_Width, uint32_t m_Height)
  //{
  //  m_SpriteRenderPass = SpriteRenderPass(m_RenderingAPI, m_ShaderLibrary);
  //
  //  AttachmentDescriptor colorAttachmentDesc{};
  //  colorAttachmentDesc.SizeX = m_Width;
  //  colorAttachmentDesc.SizeY = m_Height;
  //  AttachmentDescriptor depthAttachmentDesc{};
  //  depthAttachmentDesc.SizeX = m_Width;
  //  depthAttachmentDesc.SizeY = m_Height;
  //  depthAttachmentDesc.InternalFormat = INTERNAL_FORMAT_DEPTH_COMPONENT_32;
  //  
  //  m_PresentTarget = Framebuffer::Create(*this, { colorAttachmentDesc, depthAttachmentDesc });
  //
  //}
  //
  //void RenderGraph::Render(Scene& scene, Camera camera, glm::vec4 clearColor)
  //{
  //
  //  Ref<Framebuffer> output = m_SpriteRenderPass.Render(scene, camera, clearColor);
  //
  //  m_RenderingAPI->BlitFramebuffer
  //  (
  //    *output,
  //    output->GetDescriptor(COLOR_ATTACHMENT_0).SizeX,
  //    output->GetDescriptor(COLOR_ATTACHMENT_0).SizeY,
  //    *m_PresentTarget,
  //    m_PresentTarget->GetDescriptor(COLOR_ATTACHMENT_0).SizeX,
  //    m_PresentTarget->GetDescriptor(COLOR_ATTACHMENT_0).SizeY
  //  );
  //  m_RenderingAPI->UnbindFramebuffer();
  //}
  //
  //RendererID RenderGraph::GetLatestDisplayReadyTexture()
  //{
  //  return (*m_PresentTarget)[COLOR_ATTACHMENT_0];
  //}
  //
  //void RenderGraph::Resize(uint32_t m_Width, uint32_t m_Height)
  //{
  //  m_SpriteRenderPass.Resize(m_Width, m_Height);
  //
  //  if (m_PresentTarget->GetDescriptor(COLOR_ATTACHMENT_0).SizeX == m_Width && m_PresentTarget->GetDescriptor(COLOR_ATTACHMENT_0).SizeY == m_Height)
  //    return;
  //
  //  auto attachmentDescriptors = m_PresentTarget->GetAllDescriptors();
  //  for (auto& descriptor : attachmentDescriptors)
  //  {
  //    descriptor.SizeX = m_Width;
  //    descriptor.SizeY = m_Height;
  //  }
  //  m_PresentTarget = Framebuffer::Create(*this, { attachmentDescriptors[0], attachmentDescriptors[Framebuffer::MAX_COLOR_ATTACHMENTS] });
  //
  //  m_RenderingAPI->SetViewport(0, 0, m_Width, m_Height);
  //}

}
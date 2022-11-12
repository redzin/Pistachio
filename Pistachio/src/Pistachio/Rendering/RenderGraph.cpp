#include "pch.h"
#include "RenderGraph.h"


namespace Pistachio
{

  /////////////////////////////////////////////////////////
  /// RenderGraph
  /////////////////////////////////////////////////////////


  void RenderGraph::AddPass(Ref<RenderPass> renderPass)
  {
    m_RenderPasses.push_back(renderPass);
  }

  static const std::set<Capability> SetOfAllCapabilities
  {
    Capability::CullFace,
    Capability::DepthTest,
    Capability::ScissorTest,
    Capability::StencilTest,
    Capability::Blend
  };

  static Hash previousShaderHash = 0;
  static RendererID previousFramebuffer = -1;
  void RenderGraph::Render(Device& device, uint32_t viewport_width, uint32_t viewport_height)
  {
    for (const auto& pass : m_RenderPasses)
    {
      if (pass->m_CommandBuffer.size() < 1)
        continue;

      device.BeginNewFrame();

      Framebuffer& framebuffer = device.RequestFramebuffer(pass->m_AttachmentOuputs);
      
      std::vector<Capability> disabledCapabilities;
      std::set_symmetric_difference(
        pass->m_RenderState.Capabilities.begin(), pass->m_RenderState.Capabilities.end(),
        SetOfAllCapabilities.begin(), SetOfAllCapabilities.end(),
        std::back_inserter(disabledCapabilities));
      
      device.m_RenderingAPI->DepthMask(pass->m_RenderState.DepthMask);
      device.m_RenderingAPI->SetDepthFunc(pass->m_RenderState.DepthFunction);
      device.m_RenderingAPI->SetBlendFunction(pass->m_RenderState.BlendState.SourceBlendFunction, pass->m_RenderState.BlendState.DestinationBlendFunction);

      device.m_RenderingAPI->SetViewport(0, 0, viewport_width, viewport_height);
      
      Hash shaderHash = GetHash(pass->m_Shader);
      if (pass->m_Shader.Path != "" && shaderHash != previousShaderHash) // only switch shader if necessary
        device.m_RenderingAPI->UseShaderProgram(device.RequestShader(pass->m_Shader)->RendererID);

      if (previousFramebuffer != framebuffer) // only switch framebuffer if necessary
        device.m_RenderingAPI->BindFramebuffer(framebuffer);

      for (const auto& cap : disabledCapabilities)
        device.m_RenderingAPI->Disable(cap);

      for (const auto& cap : pass->m_RenderState.Capabilities)
        device.m_RenderingAPI->Enable(cap);

      for (const auto& cmd : pass->m_CommandBuffer)
        cmd(device, *device.m_RenderingAPI);

      pass->m_CommandBuffer.clear();
      device.m_RenderingAPI->UnbindFramebuffer();

    }

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

  RenderPassState::RenderPassState(std::initializer_list<Capability> capabilities, Pistachio::DepthFunction depthFunction, Pistachio::BlendState blendState, bool depthMask)
    : Capabilities(GetCapabilitySet(std::move(capabilities))), DepthFunction(depthFunction), BlendState(blendState), DepthMask(depthMask)
  {
  }


  /////////////////////////////////////////////////////////
  /// RenderPass
  /////////////////////////////////////////////////////////

  void RenderPass::SetRenderState(RenderPassState& state)
  {
    m_RenderState = state;
  }

  void RenderPass::SetShaderProgram(ShaderDescriptor& shaderDescriptor)
  {
    m_Shader = shaderDescriptor;
  }

  void RenderPass::AddAttachmentOutput(Ref<Attachment> attachment)
  {
    m_AttachmentOuputs.push_back(attachment);
  }

  void RenderPass::ClearAttachmentOutputs()
  {
    m_AttachmentOuputs.clear();
  }

  void RenderPass::RecordCommandBuffer(std::function<void(Device&, RenderingAPI&)> commands)
  {
    m_CommandBuffer.push_back(commands);
  }

}
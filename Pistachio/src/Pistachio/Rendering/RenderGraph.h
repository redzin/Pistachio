#pragma once
#include "pch.h"

#include "Pistachio/Scene/Camera/Camera.h"

namespace Pistachio
{
  /*
  * 
  * Notes on Themasiter's Granite:
  * 
  * ****************
  * GPU side thingies (RendererIDs, etc)
  * ****************
  * Command buffer records GPU commands, is assigned a specific thread on creation - owned by device
  * RenderCommandBuffer should request a framebuffer from the device when needed, and the device should keep them in a "temporary" hashmap (ring buffer)
  * 
  * 
  * ****************
  * CPU info used to figure out the best order of operations and organise the GPU commands
  * ****************
  * RenderGraph - global constructor of renderpasses and render pass dependencies
  * RenderResource - Info about texture (samplers and attachments!) and buffer dependencies, transient state info, etc., owned by RenderGraph
  * RenderPasses owned by RenderGraph - keeps pointers to related resources in RenderGraph
  * AttachmentDescriptors owned by RenderPass
  * RenderGraph holds pointers (Ref<T>) to all related physical resources (attachments, samplers, buffers, etc.) which are owned and alocated by the device
  * 
  * 
  * ****************
  * CPU side data - meshes, etc
  * ****************
  * Scene root keeps global info like skybox
  * Scene hierarchy is DAG
  * Each node keeps an list of meshes (including materials), a transform and a list of sub-scenes
  * Store list of mesh data on nodes contiguously through std::vector, but no contiguousness between different sub-scenes (std::vector<Mesh> with Mesh holding all data relating to the mesh on its stack)
  * RenderableComponent type is a pointer to a Mesh which holds both cpu data (as described above) and a handle to a relevant GPU buffers
  * Data is transfered to GPU upon scene load and RenderableComponent objects are created
  * 
  * 
  * ****************
  * Buffers and Textures
  * ****************
  * Need 4 different objects, at least:
  *   One for RenderGraph (RenderResource) to figure out dependencies and transience - owned by RenderGraph
  *   One for storing physical data on cpu ram - owned by the Scene object?, stored in ObjectPool<>
  *   One for storing physical data on gpu ram - owned by Device, stored in ObjectPool<>
  *   Descriptor for allocating data on gpu - how best to transfer data? Read Granite blog post!
  * 
  */

  class Device;
  class RenderGraph;
  

  struct BlendState
  {
    BlendFunction SourceBlendFunction = BlendFunction::SrcAlpha;
    BlendFunction DestinationBlendFunction = BlendFunction::OneMinusSrcAlpha;
  };

  struct RenderPassState
  {
    std::set<Capability> Capabilities { Capability::DepthTest }; // Capabilities not in this set will be explicitly disabled for the renderpass
    bool DepthMask = true;
    DepthFunction DepthFunction = DepthFunction::LEqual;
    BlendState BlendState;

    RenderPassState() = default;
    RenderPassState(RenderPassState&) = default; // support copying
    RenderPassState& operator=(RenderPassState&) = default; // support copying
    RenderPassState(RenderPassState&&) = default; // support moving
    RenderPassState& operator=(RenderPassState&&) noexcept = default; // support moving
    ~RenderPassState() = default;

    RenderPassState(
      std::initializer_list<Capability> capabilities,
      Pistachio::DepthFunction depthFunction = DepthFunction::LEqual,
      Pistachio::BlendState blendState = Pistachio::BlendState(),
      bool depthMask = true
    );

  };

  struct AttachmentInput
  {
    Ref<Attachment> Attachment;
    SamplerDescriptor SamplerDescriptor;
  };

  struct AttachmentInputResource : public RenderResourceBase
  {
  };

  class RenderPass
  {
    friend class RenderGraph;
  public:
    RenderPass(const std::string& semanticName) : SemanticName(semanticName) {}
    RenderPass(RenderPass&) = default; // support copying
    RenderPass& operator=(RenderPass&) = default; // support copying
    RenderPass(RenderPass&&) = default; // support moving
    RenderPass& operator=(RenderPass&&) noexcept = default; // support moving
    ~RenderPass() = default;

    void SetRenderState(RenderPassState& state);
    void SetShaderProgram(const std::string& shaderName);
    void AddAttachmentOutput(Ref<Attachment> attachment);
    void RecordCommandBuffer(std::function<void(Device&, RenderingAPI&)> commands);
    
  private:
    std::string SemanticName;

    RenderPassState m_RenderState;
    Ref<ShaderResource> m_Shader = nullptr;
    std::vector<Ref<Attachment>> m_AttachmentOuputs;
    std::vector<std::function<void(Device&, RenderingAPI&)>> m_CommandBuffer; // todo: write a real command buffer
  };



  // RenderGraph update:
  // RenderGraph can AddPass(...) which just adds the RenderPass to a vector (more or less) in order to associate that RenderPass to the render graph
  //    Maybe also a version which takes an initializer list of render passes to make it more user friendly
  // RenderGraph can Bake() which looks at all currently associated render passes and figures out the "optimal" dependency graph based on the current state of the render passes.
  //    Step 1) make RenderResources that hold information about which resources are used/read/written in which passes
  //    Step 2) ???
  //    Step 3) Profit
  // RenderGraph can RemoveRenderPass(...) which removes a render pass. (by name or by const RenderPass&)
  // RenderGraph can Reset() which removes all currently added render passes and resources.
  // RenderGraph does not auto bake when render passes are added. Baking is done deliberately!
  // 
  // RenderGraph has Render function. Should it just expose access to the graph nodes and let execution of the passes be handled externally? Consideration for later...

  class RenderGraph
  {
  public:
    RenderGraph() = default;

    void AddPass(Ref<RenderPass> renderPass);
    void Render(Device& device, uint32_t viewport_width, uint32_t viewport_height); // does this functionality belong here?
    
  private:
    bool m_IsCooked = false;

    std::vector<Ref<RenderPass>> m_RenderPasses;

    std::vector<Ref<ShaderResource>> m_ShaderResources;
    std::vector<Ref<AttachmentResource>> m_Attachments;
    std::vector<Ref<SamplerResource>> m_Samplers;
    std::vector<Ref<BufferResource>> m_Buffers;

  };

}


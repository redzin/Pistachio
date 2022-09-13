#pragma once
#include "pch.h"

#include "Pistacio/Scene/Camera/Camera.h"

namespace Pistacio
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
  //class RenderPass;
  class RenderGraph;
  
  struct RenderShaderResource
  {
    std::string Name;
    std::unordered_set<std::string> UsedInPasses;
  };

  struct RenderResource
  {
    std::string Name;
    std::unordered_set<std::string> ReadInPasses;
    std::unordered_set<std::string> WrittenInPasses;
  };

  struct VertexBufferResource
  {
    std::string Name;
    std::unordered_set<std::string> UsedInPasses;
    BufferMemoryLayout Layout;
  };

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
    RenderPassState(
      std::initializer_list<Capability> capabilities,
      Pistacio::DepthFunction depthFunction = DepthFunction::LEqual,
      Pistacio::BlendState blendState = Pistacio::BlendState(),
      bool DepthMask = true
    );

  };


  class RenderPass
  {
    friend class Device;
    friend class RenderGraph;
    friend class ObjectPool<RenderPass>;
  public:
    //RenderPass(const RenderPass& other)
    //: m_RenderGraph(other.m_RenderGraph),
    //  m_Name(other.m_Name),
    //  m_DepthStencilInput(other.m_DepthStencilInput),
    //  m_DepthStencilOutput(other.m_DepthStencilOutput),
    //  m_ResolveOutput(other.m_ResolveOutput),
    //  m_IndexBufferInput(other.m_IndexBufferInput),
    //  m_VertexBufferInputs(other.m_VertexBufferInputs),
    //  m_ColorAttachmentOuputs(other.m_ColorAttachmentOuputs),
    //  m_ColorAttachmentInputs(other.m_ColorAttachmentInputs),
    //  m_SamplerInputs(other.m_SamplerInputs),
    //  m_CommandBuffer(other.m_CommandBuffer),
    //  m_ShaderResource(other.m_ShaderResource)
    //{
    //}


    RenderPass& operator=(RenderPass& other) { return other; }

    void SetShaderProgram(const std::string&& SemanticName);

    void SetDepthStencilInput(const std::string& SemanticName);
    void SetDepthStencilOutput(const std::string SemanticName);

    void AddAttachmentInput(const std::string& SemanticName);
    void AddAttachmentOutput(const std::string& SemanticName);

    void AddSamplerInput(const std::string& SemanticName);

    void AddVertexBufferInput(const std::string& SemanticName, const BufferMemoryLayout& vertexLayout);
    void AddIndexBufferInput(const std::string& SemanticName);
    void AddResolveOutput(const std::string& SemanticName);
    
    void SetRenderState(RenderPassState&& state);

    void RecordCommandBuffer(std::function<void(RenderingAPI&, RendererID)>);

    void Resize(Device& device, uint32_t m_Width, uint32_t m_Height);

  private:
    RenderPass(RenderGraph& renderGraph, const std::string& SemanticName) : m_Name(SemanticName), m_RenderGraph(renderGraph) {}
    RenderGraph& m_RenderGraph;
    std::string m_Name;
    RenderPassState m_RenderState;

    RenderResource* m_DepthStencilOutput = nullptr;
    RenderResource* m_DepthStencilInput = nullptr;
    std::vector<RenderResource*> m_ColorAttachmentOuputs;
    std::vector<RenderResource*> m_ColorAttachmentInputs;
    std::vector<RenderResource*> m_SamplerInputs;

    RenderResource* m_IndexBufferInput = nullptr;
    std::vector<VertexBufferResource*> m_VertexBufferInputs;

    std::vector<RenderResource*> m_ResolveOutputs;

    RenderShaderResource* m_ShaderResource = nullptr;

    std::function<void(RenderingAPI&, RendererID)> m_CommandBuffer; // todo: write a real command buffer, maybe
  };


  class RenderGraph
  {
    friend class RenderPass;
  public:
    RenderGraph() = default;
    
    RenderPass* AddPass(const std::string& SemanticName);
    void Render(Device& device, Window& window, uint32_t viewport_width, uint32_t viewport_height);
    
  private:
    void Cook(Device& device); // <- Prepare for rendering. Make sure all the physical resource are available, etc.
    bool m_IsCooked = false;

    RenderShaderResource* RequestShaderResource(const std::string& SemanticName);
    RenderResource* RequestResource(const std::string& SemanticName);
    VertexBufferResource* RequestVertexBufferResource(const std::string& SemanticName);

    std::vector<RenderPass*> m_CookedRenderPasses;
    std::unordered_map<std::string, RenderPass*> m_RenderPasses;
    std::unordered_map<std::string, RenderShaderResource*> m_ShaderResources;
    std::unordered_map<std::string, RenderResource*> m_RenderResources;
    std::unordered_map<std::string, VertexBufferResource*> m_VertexBufferResources;

    ObjectPool<RenderShaderResource> m_ShaderResourcePool;
    ObjectPool<RenderResource> m_RenderResourcePool;
    ObjectPool<VertexBufferResource> m_VertexBufferResourcePool;
    ObjectPool<RenderPass> m_RenderPassPool;

  };

}


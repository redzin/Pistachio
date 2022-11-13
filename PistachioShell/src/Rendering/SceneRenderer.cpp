#include "SceneRenderer.h"

namespace Pistachio
{
  
  void RenderScene(SpritePassData& spritePassData, Device& device, Scene& scene, Camera& camera, glm::vec4 clearColor, uint32_t viewportWidth, uint32_t viewportHeight)
  {

    ResizeAttachments(spritePassData, device, viewportWidth, viewportHeight);
    BeginFrame(spritePassData, clearColor, camera);

    auto spriteView = scene.GetGroup<>(Scene::GroupGet<RenderableSpriteComponent>, Scene::GroupExclude<TransparentRenderableComponent>);
    for (EntityID spriteId : spriteView)
    {
      RenderableSpriteComponent& spriteComponent = spriteView.get<RenderableSpriteComponent>(spriteId);
      Draw(spriteComponent, camera, device, spritePassData);
      
    }

    auto transparentSpriteView = scene.GetView<RenderableSpriteComponent, TransparentRenderableComponent>();
    for (EntityID spriteId : transparentSpriteView)
    {
      RenderableSpriteComponent& spriteComponent = transparentSpriteView.get<RenderableSpriteComponent>(spriteId);
      Draw(spriteComponent, camera, device, spritePassData);
    }
  }

  Transform ComputeTransformRecursively(EntityID leafId, Scene& scene)
  {
    auto transformView = scene.GetView<Transform>() ;
    Transform& transform = transformView.get<Transform>(leafId);

    auto relationshipView = scene.GetView<SceneEntityRelationship>();

    if (!relationshipView.contains(leafId))
      return transform;

    SceneEntityRelationship relationship = relationshipView.get<SceneEntityRelationship>(leafId);
    if (relationship.ParentID == entt::null)
      return transform;
    
    Transform parentTransform = ComputeTransformRecursively(relationship.ParentID, scene);

    Transform result = parentTransform * transform;
    
    return result;
    
  }

  void RenderScene(PBRPassData& pbrPassData, Device& device, RenderGraph& renderGraph, Scene& scene, Camera& camera, glm::vec4 clearColor, uint32_t viewportWidth, uint32_t viewportHeight)
  {
    ResizeAttachments(pbrPassData, device, viewportWidth, viewportHeight);
    BeginFrame(pbrPassData, clearColor, camera);

    //auto modelsView = scene.GetGroup<PBRMetallicRoughnessMaterial>(Scene::GroupGet<StaticMesh, Transform>);
    auto modelsView = scene.GetView<Model, Transform, SemanticNameComponent>();
    for (EntityID modelId : modelsView)
    {
      SemanticNameComponent& name = modelsView.get<SemanticNameComponent>(modelId);
      Model& model = modelsView.get<Model>(modelId);

      Transform transform = ComputeTransformRecursively(modelId, scene);
      
      for (auto& materialMesh : model.Submeshes)
      {
        PBRMetallicRoughnessMaterial& material = materialMesh.Material;
        StaticMesh& mesh = materialMesh.Mesh;

        Hash materialHash = GetHash(material);

        if (pbrPassData.RenderPasses.count(materialHash) < 1)
        {
          AddNewRenderPass(pbrPassData, material, mesh, renderGraph);
        }

        Draw(mesh, material, transform, camera, device, pbrPassData);

      }

      
    }
  }

  std::map<std::string, Ref<Attachment>> SceneRenderer::GetDisplayReadyAttachments()
  {
    std::map<std::string, Ref<Attachment>> attachments;

    attachments["PBRPass_ColorAttachment"] = m_PBRPassData.ColorAttachment;
    attachments["PBRPass_DepthAttachment"] = m_PBRPassData.DepthAttachment;
    attachments["SpritePass_ColorAttachment"] = m_SpritePassData.ColorAttachment;
    attachments["SpritePass_DepthAttachment"] = m_SpritePassData.DepthAttachment;

    return attachments;
  }

  void SceneRenderer::Init(Device& device, std::unordered_map<std::string, uint32_t> textureMap, uint32_t maxSpriteCount, uint32_t samplerWidth, uint32_t samplerHeight)
  {

    m_SpritePassData = CreateSpritePass(device, m_RenderGraph, textureMap, maxSpriteCount, samplerWidth, samplerHeight);
    m_PBRPassData = CreatePBRPass(device, m_RenderGraph);
    
  }

  void SceneRenderer::Render(Device& device, Scene& scene, Camera& camera, uint32_t viewportWidth, uint32_t viewportHeight, glm::vec4 clearColor)
  {

    RenderScene(m_SpritePassData, device, scene, camera, clearColor, viewportWidth, viewportHeight);
    RenderScene(m_PBRPassData, device, m_RenderGraph, scene, camera, clearColor, viewportWidth, viewportHeight);

    m_RenderGraph.Render(device, viewportWidth, viewportHeight);

  }

  
}

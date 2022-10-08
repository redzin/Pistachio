#pragma once
#include "Pistachio/Rendering/RenderGraph.h"
#include "SceneComponents/SceneComponents.h"
#include "SceneComponents/SpriteComponent.h"
#include "SceneComponents/Camera/Camera.h"
#include "SceneComponents/Camera/CameraOrbitController.h"
#include "Rendering/SceneRenderer.h"
#include "ImGui/MainDockspace.h"
#include "ImGui/RenderViewport.h"
#include "ImGui/PerformanceTracker.h"

namespace Pistachio
{

  class EditorLayer : public Layer
  {
  private:

    //Scene and rendering variables
    Scene m_Scene;
    Scope<SceneRenderer> m_SceneRenderer;
    glm::vec4 m_ClearColor{ 0.65f, 0.88f, 0.92f, 1.00f };

    // Camera variables
    Ref<Camera> m_Camera;
    CameraOrbitController m_CameraOrbitController;
    inline static constexpr float fovY = 70;
    inline static constexpr float zNear = 0.1f;
    inline static constexpr float zFar = 1000.0f;

    // ImGui Panels
    Scope<MainDockspace> m_Dockspace;
    Viewport m_Viewport;
    PerformanceTracker m_PerformanceTracker;

    // ImGui Debug Panel
    //std::vector<const char*> m_PresentTextureNames;

  public:

    const char* GetName() const { return "EditorLayer"; }

    EditorLayer(uint32_t width, uint32_t height)
      : m_Viewport(Viewport(width, height))
    {
    }

    void OnAttach(Window& window, EventLibrary& eventLib) override;

    void OnDetach(Window& window, EventLibrary&) override;

    void OnUpdate(std::chrono::duration<float> dt) override;

    void OnRender(Device& device, Window& window) override;

    void OnGuiRender(Window& window, EventLibrary& eventLibrary) override;
  };


}


//static void SortTransparentSprites(Scene& scene, Camera camera)
//{
//  scene.Sort<TransparentRenderableComponent>([&scene, &camera](const EntityID lhs, const EntityID rhs)
//    {
//      SceneEntity lhsEntity(scene, std::move(lhs));
//      SceneEntity rhsEntity(scene, std::move(rhs));
//      auto lhsSprite = lhsEntity.GetComponent<SpriteRenderableComponent>();
//      auto rhsSprite = rhsEntity.GetComponent<SpriteRenderableComponent>();
//
//      return glm::length(glm::vec4(camera.position, 1.0) - lhsSprite.Transform[3]) > glm::length(glm::vec4(camera.position, 1.0) - rhsSprite.Transform[3]);
//    }
//  );
//  scene.Sort<SpriteRenderableComponent, TransparentRenderableComponent>();
//}

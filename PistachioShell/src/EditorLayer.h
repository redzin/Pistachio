#pragma once
#include "Pistachio/Rendering/RenderGraph.h"
#include "Scene/Scene.h"
#include "Scene/SceneComponents.h"
#include "Scene/SpriteComponent.h"
#include "Scene/SceneLoader.h"
#include "Scene/Camera/Camera.h"
#include "Scene/Camera/CameraOrbitController.h"
#include "Scene/SceneComponents.h"
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
    SceneLoader m_SceneLoader;
    Scope<SceneRenderer> m_SceneRenderer;
    glm::vec4 m_ClearColor{ 0.65f, 0.88f, 0.92f, 1.00f };

    Ref<Camera> m_EditorCamera;
    CameraOrbitController m_EditorCameraOrbitController;
    inline static constexpr float fovY = 70;
    inline static constexpr float zNear = 0.1f;
    inline static constexpr float zFar = 1000.0f;

    // ImGui Panels
    // Camera variables
    int32_t m_SelectedSceneIndex = -1;
    int32_t m_SelectedSceneCameraIndex = 0;
    std::unordered_map<int32_t, EntityID> m_SelectedSceneCameraEntityMap;
    Camera& GetActiveCamera();
    CameraOrbitController& GetActiveCameraController();
    std::vector<Scene> m_Scenes;
    Scope<MainDockspace> m_Dockspace;
    Viewport m_Viewport;
    PerformanceTracker m_PerformanceTracker;

    // ImGui Debug Panel
    //std::vector<const char*> m_PresentTextureNames;

  public:

    const char* GetName() const { return "Main Editor Layer"; }

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

#pragma once
#include "Pistachio/Rendering/RenderGraph.h"
#include "Scene/Scene.h"
#include "Scene/SceneComponents.h"
#include "Scene/SpriteComponent.h"
#include "Scene/Camera/Camera.h"
#include "Scene/Camera/CameraOrbitController.h"
#include "Scene/SceneLoader.h"
#include "Rendering/SceneRenderer.h"
#include "ImGui/ImGuiPanels.h"

namespace Pistachio
{

  class EditorLayer : public Layer
  {
  private:

    //Scene and rendering variables
    SceneLoader m_SceneLoader;
    Scope<SceneRenderer> m_SceneRenderer;
    std::vector<Scene> m_Scenes;

    //Ref<Camera> m_EditorCamera;
    //CameraOrbitController m_EditorCameraOrbitController;
    inline static constexpr float fovY = 70;
    inline static constexpr float zNear = 0.05f;
    inline static constexpr float zFar = 1000.0f;

    //Camera& GetActiveCamera(CameraPanel& cameraPanel);
    //CameraOrbitController& GetActiveCameraController(CameraPanel& cameraPanel);

    // ImGui Panels
    MainDockspace m_Dockspace;

  public:
    const char* GetName() const { return "Main Editor Layer"; }

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

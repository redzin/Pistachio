#pragma once
#include "../Scene/Scene.h"
#include "../Scene/SceneComponents.h"
#include "../Scene/Camera/Camera.h"
#include "../Scene/Camera/CameraOrbitController.h"

#include <Pistachio.h>

namespace Pistachio
{

  enum class ExampleScene
  {
    None = 0,
    OneMillionSprites,
    Cube,
    Box,
    BoxVertexColors,
    Duck,
    MetalRoughSpheres,
    EnvironmentTest,
    FlightHelmet,
    Buggy,
    ABeautifulGame,
    Sponza
  };

  struct ExampleSceneLoadEvent
  {
    ExampleScene ExampleScene;
  };

  struct ClearColorPicker
  {
    ImVec4 ImguiClearColor{ 0.7f, 0.85f, 0.92f, 1.00f };
    glm::vec4 ClearColor{ 0.7f, 0.85f, 0.92f, 1.00f };
  };
  void Render(ClearColorPicker& performanceTracker);

  struct PerformanceTracker
  {
    bool VSync = true;
    std::string Name = "Performance";
  };
  void Render(PerformanceTracker& performanceTracker, Window& window);

  struct RenderGraphAttachmentPanel
  {
    std::string Name = "Render Graph Attachments";
    std::vector<const char*> AttachmentNames;
    int SelectedAttachmentIndex = 0;
    std::map<std::string, Ref<Attachment>> Attachments;
  };
  void Render(RenderGraphAttachmentPanel& renderGraphAttachmentPanel, std::map<std::string, Ref<Attachment>> sceneRenderer);

  struct CameraPanel;
  struct ScenePanel
  {
    std::string Name = "Scene Panel";
    int SelectedSceneIndex = -1;
  };
  void Render(ScenePanel& scenePanel, CameraPanel& cameraPanel, std::vector<Scene>& scenes);

  struct CameraPanel
  {
    std::string Name = "Camera Panel";
    int32_t SelectedSceneCameraIndex = 0;
    std::unordered_map<int32_t, EntityID> SelectedSceneCameraEntityMap;
  };
  Camera& GetActiveCamera(CameraPanel& cameraPanel, Scene& scene);
  CameraOrbitController& GetActiveCameraController(CameraPanel& cameraPanel, Scene& scene);
  void Render(CameraPanel& cameraPanel, ScenePanel& scenePanel, std::vector<Scene>& scenes);

  struct ViewportMouseClickEvent : MouseClickEvent {};
  struct ViewportMouseMoveEvent : MouseMoveEvent {};
  struct ViewportMouseScrollEvent : MouseScrollEvent {};
  struct Viewport
  {
    uint32_t Width;
    uint32_t Height;
    std::string Name = "Viewport";
  };
  bool OnViewportMouseClickEvent(Viewport& viewport, ViewportMouseClickEvent& e, Window& window, CameraOrbitController& cameraOrbitController);
  bool OnViewportMouseMoveEvent(Viewport& viewport, ViewportMouseMoveEvent& e, CameraOrbitController& cameraOrbitController);
  bool OnViewportScrollEvent(Viewport& viewport, ViewportMouseScrollEvent& e, CameraOrbitController& cameraOrbitController);
  void Render(Viewport& viewport, RenderGraphAttachmentPanel& attachmentPanel, ScenePanel& scenePanel, Window& window, CameraOrbitController& cameraOrbitController);

  struct PBRShaderPanel
  {
    std::string Name = "PBR Shader";
    PBRShaderOverrides PBRShaderOverrides;
    ClearColorPicker ClearColorPicker;
  };
  void Render(PBRShaderPanel& pbrShaderPanel);

  struct MainDockspace
  {
    PerformanceTracker PerformanceTracker;
    Viewport RenderViewport;
    RenderGraphAttachmentPanel RenderGraphAttachmentPanel;
    ScenePanel ScenePanel;
    CameraPanel CameraPanel;
    PBRShaderPanel PBRShaderPanel;

    std::string Name = "Dockspace";

  };
  void Render(MainDockspace& dockspace, EventLibrary& eventLibrary);

}


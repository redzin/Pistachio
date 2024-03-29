#include "EditorLayer.h"

namespace Pistachio
{

  void AddEditorCameraToScene(Device& device, Scene& scene, float fovY, uint32_t width, uint32_t height, float zNear, float zFar)
  {
    SceneEntity editorCameraEntity = scene.CreateEntity();
    editorCameraEntity.AddComponent<EditorCameraTag>();
    editorCameraEntity.AddComponent<SemanticNameComponent>("Editor Camera");
    CameraOrbitController& camOrbitController = editorCameraEntity.AddComponent<CameraOrbitController>();
    editorCameraEntity.AddComponent<Camera>(
      std::move(
        camOrbitController.CreatePerspectiveCamera(
          device,
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, -1.0f),
          5.0f,
          fovY,
          width,
          height,
          zNear,
          zFar
        )
      )
      );
  }

  void EditorLayer::OnAttach(Window& window, EventLibrary& eventLib)
  {
    eventLib.Register<ExampleSceneLoadEvent>();

    Device& device = window.GetDevice();
    m_SceneRenderer = CreateScope<SceneRenderer>();

    std::string texChechkerboardPath = "assets/textures/Checkerboard.png";
    std::string texChernoPath = "assets/textures/ChernoLogo.png";
    std::string texCherno2Path = "assets/textures/ChernoLogo2.png";
    std::unordered_map<std::string, uint32_t> textureMap = { {texChernoPath, 0}, {texCherno2Path, 1} };

    int maxSpritesSqrt = glm::pow(2, 10);
    float shift = maxSpritesSqrt / 2.0;
    m_SceneRenderer->Init(device, textureMap, glm::pow(maxSpritesSqrt, 2), 1024, 1024);

    eventLib.Subscribe<KeyEvent>([&eventLib](KeyEvent e)
      {
        if (e.key == Input::KeyCode::Escape && e.action == Input::ButtonAction::KeyPressed)
          eventLib.Publish<WindowCloseEvent>(WindowCloseEvent{});
        return true;
      }
    );

    eventLib.Subscribe<ExampleSceneLoadEvent>([&eventLib, &device, textureMap, maxSpritesSqrt, shift, texChernoPath, texCherno2Path, this](ExampleSceneLoadEvent e)
      {

        m_Scenes.clear();
        m_SceneRenderer->Init(device, textureMap, glm::pow(maxSpritesSqrt, 2), 1024, 1024);

        switch (e.ExampleScene)
        {
        case ExampleScene::None:
          break;

        case ExampleScene::Cube:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/Cube/glTF/Cube.gltf");
          m_Dockspace.ScenePanel.SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::Box:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/Box/glTF/Box.gltf");
          m_Dockspace.ScenePanel.SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::BoxVertexColors:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/BoxVertexColors/glTF/BoxVertexColors.gltf");
          m_Dockspace.ScenePanel.SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::Duck:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/Duck/glTF/Duck.gltf");
          m_Dockspace.ScenePanel.SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::MetalRoughSpheres:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/MetalRoughSpheres/glTF/MetalRoughSpheres.gltf");
          m_Dockspace.ScenePanel.SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;
          
        case ExampleScene::EnvironmentTest:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/EnvironmentTest/glTF/EnvironmentTest.gltf");
          m_Dockspace.ScenePanel.SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::FlightHelmet:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/FlightHelmet/glTF/FlightHelmet.gltf");
          m_Dockspace.ScenePanel.SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::Buggy:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/Buggy/glTF/Buggy.gltf");
          m_Dockspace.ScenePanel.SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::ABeautifulGame:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/ABeautifulGame/glTF/ABeautifulGame.gltf");
          m_Dockspace.ScenePanel.SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::Sponza:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/Sponza/glTF/Sponza.gltf");
          m_Dockspace.ScenePanel.SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::OneMillionSprites:

          m_Scenes.push_back(Scene("One Million Sprites"));

          SceneEntity sceneCamera = m_Scenes[0].CreateEntity();
          sceneCamera.AddComponent<PrimarySceneCameraTag>();
          sceneCamera.AddComponent<CameraOrbitController>();
          sceneCamera.AddComponent<SemanticNameComponent>("Scene Camera");
          CameraOrbitController& camOrbitController = sceneCamera.GetComponent<CameraOrbitController>();
          sceneCamera.AddComponent<Camera>(
            std::move(
              camOrbitController.CreatePerspectiveCamera(
                device,
                glm::vec3(1.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, -1.0f),
                5.0f,
                fovY,
                m_Dockspace.RenderViewport.Width,
                m_Dockspace.RenderViewport.Height,
                zNear,
                zFar
              )
            )
          );

          SceneEntity spriteChernos2 = m_Scenes[0].CreateEntity();
          spriteChernos2.AddComponent<SemanticNameComponent>("Chernos2");
          spriteChernos2.AddComponent<RenderableSpriteComponent>(device, glm::pow(maxSpritesSqrt, 2));

          RenderableSpriteComponent& spriteComponent2 = spriteChernos2.GetComponent<RenderableSpriteComponent>();
          glm::mat4* transforms2 = (glm::mat4*)spriteComponent2.TransformBuffer->MemoryPtr;
          uint32_t* samplerIndices2 = (uint32_t*)spriteComponent2.TexCoordIndexBuffer->MemoryPtr;
          
          Timer t;
          for (int i = 0; i < maxSpritesSqrt; i++)
          {
            for (int j = 0; j < maxSpritesSqrt; j++)
            {
              transforms2[i + j * maxSpritesSqrt] = glm::translate(glm::mat4(1.0f), glm::vec3(i - shift, j - shift, -0.5f));
              samplerIndices2[i + j * maxSpritesSqrt] = textureMap.at(std::string(texCherno2Path));
            }
          }
          int loadingTime = t.getElapsed<std::chrono::milliseconds>();

          PSTC_CORE_INFO("Sprite loading time: {0}ms", loadingTime);

          //SceneEntity spriteChernos = m_Scene.CreateEntity();
          //spriteChernos.AddComponent<SemanticNameComponent>("Chernos");
          //spriteChernos.AddComponent<RenderableSpriteComponent>(device, glm::pow(max, 2));
          //spriteChernos.AddComponent<TransparentRenderableComponent>();
          //
          //RenderableSpriteComponent& spriteComponent = spriteChernos.GetComponent<RenderableSpriteComponent>();
          //glm::mat4* transforms = (glm::mat4*)spriteComponent.TransformBuffer->MemoryPtr;
          //uint32_t* samplerIndices = (uint32_t*)spriteComponent.TexCoordIndexBuffer->MemoryPtr;
          //for (int i = 0; i < max; i++)
          //{
          //  for (int j = 0; j < max; j++)
          //  {
          //    transforms[i + j * max] = glm::translate(glm::mat4(1.0f), glm::vec3(i - shift, j - shift, 0.5f));
          //    samplerIndices[i + j * max] = textureMap[texChernoPath];
          //  }
          //}


          break;
        }

        for (auto& scene : m_Scenes)
          AddEditorCameraToScene(device, scene, fovY, m_Dockspace.RenderViewport.Width, m_Dockspace.RenderViewport.Width, zNear, zFar);

        return true;

      }
    );

    eventLib.Publish< ExampleSceneLoadEvent>({ ExampleScene::Sponza });

    PSTC_INFO("{0} attached!", GetName());
  }

  void EditorLayer::OnDetach(Window& window, EventLibrary&)
  {
    PSTC_INFO("{0} detached!", GetName());
  }

  void EditorLayer::OnUpdate(std::chrono::duration<float> dt)
  {

    uint32_t viewportWidth =  m_Dockspace.RenderViewport.Width;
    uint32_t viewportHeight = m_Dockspace.RenderViewport.Height;

    if (m_Dockspace.ScenePanel.SelectedSceneIndex < 0 || m_Scenes.size() < 1 || m_Dockspace.ScenePanel.SelectedSceneIndex >= m_Scenes.size())
      return;

    Scene& scene = m_Scenes[m_Dockspace.ScenePanel.SelectedSceneIndex];

    Camera& camera = GetActiveCamera(m_Dockspace.CameraPanel, scene);
    CameraOrbitController& cameraOrbitController = GetActiveCameraController(m_Dockspace.CameraPanel, scene);

    cameraOrbitController.UpdateCamera(camera, dt);

    glm::uvec2 viewportDimensions = camera.GetViewportDimensions();

    if (viewportDimensions.x != viewportWidth || viewportDimensions.y != viewportHeight)
      camera.UpdateProjection(fovY, viewportWidth, viewportHeight, zNear, zFar);

  }

  void EditorLayer::OnRender(Device& device, Window& window)
  {
    if (m_Dockspace.ScenePanel.SelectedSceneIndex < 0 || m_Scenes.size() < 1)
      return;

    Scene& scene = m_Scenes[m_Dockspace.ScenePanel.SelectedSceneIndex];
    Camera& camera = GetActiveCamera(m_Dockspace.CameraPanel, scene);
    glm::uvec2 viewportDimensions = camera.GetViewportDimensions();

    if (m_Scenes.size() > 0)
      m_SceneRenderer->Render(device, m_Scenes[m_Dockspace.ScenePanel.SelectedSceneIndex], camera, viewportDimensions.x, viewportDimensions.y, m_Dockspace.PBRShaderPanel.PBRMaterialOverrides, m_Dockspace.PBRShaderPanel.ClearColorPicker.ClearColor);
  }

  void EditorLayer::OnGuiRender(Window& window, EventLibrary& eventLibrary)
  {

    //ImGui::ShowDemoWindow();

    Render(m_Dockspace, eventLibrary);
    Render(m_Dockspace.PerformanceTracker, window);
    Render(m_Dockspace.PBRShaderPanel);
    Render(m_Dockspace.ScenePanel, m_Dockspace.CameraPanel, m_Scenes);
    Render(m_Dockspace.RenderGraphAttachmentPanel, m_SceneRenderer->GetDisplayReadyAttachments());
    Render(m_Dockspace.RenderViewport, m_Dockspace.RenderGraphAttachmentPanel, m_Dockspace.ScenePanel, window, GetActiveCameraController(m_Dockspace.CameraPanel, m_Scenes[m_Dockspace.ScenePanel.SelectedSceneIndex]));

  }

}



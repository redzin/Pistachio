#include "EditorLayer.h"

namespace Pistachio
{

  Camera& EditorLayer::GetActiveCamera()
  {
    if (m_SelectedSceneCameraIndex < 1)
      return *m_EditorCamera;

    if (m_SelectedSceneIndex < 0 || m_SelectedSceneIndex > m_Scenes.size())
      return *m_EditorCamera;

    EntityID selectedCameraID = m_SelectedSceneCameraEntityMap[m_SelectedSceneCameraIndex];
    auto sceneCameraView = m_Scenes[m_SelectedSceneIndex].GetView<Camera>();
    for (const EntityID& id : sceneCameraView)
    {
      if(id == selectedCameraID)
        return sceneCameraView.get<Camera>(id);
    }
    
    return *m_EditorCamera;
  }

  CameraOrbitController& EditorLayer::GetActiveCameraController()
  {
    if (m_SelectedSceneCameraIndex < 1)
      return m_EditorCameraOrbitController;

    if (m_SelectedSceneIndex < 0 || m_SelectedSceneIndex > m_Scenes.size())
      return m_EditorCameraOrbitController;

    EntityID selectedCameraID = m_SelectedSceneCameraEntityMap[m_SelectedSceneCameraIndex];
    auto sceneCameraView = m_Scenes[m_SelectedSceneIndex].GetView<CameraOrbitController>();
    for (const EntityID& id : sceneCameraView)
    {
      if (id == selectedCameraID)
        return sceneCameraView.get<CameraOrbitController>(id);
    }

    return m_EditorCameraOrbitController;
    
  }

  void EditorLayer::OnAttach(Window& window, EventLibrary& eventLib)
  {
    Device& device = window.GetDevice();
    m_SceneRenderer = CreateScope<SceneRenderer>();
    m_Dockspace = CreateScope<MainDockspace>(eventLib);

    m_EditorCamera.reset(new Camera(
      std::move(
            m_EditorCameraOrbitController.CreatePerspectiveCamera(
              device,
              glm::vec3(0),
              glm::vec3(0.0f, 0.0f, -1.0f),
              5.0f,
              fovY,
              m_Viewport.GetWidth(),
              m_Viewport.GetHeight(),
              zNear,
              zFar
          )
        )
      )
    );

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
        switch (e.ExampleScene)
        {
        case ExampleScene::None:
          break;

        case ExampleScene::Duck:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/Duck/glTF/Duck.gltf");
          m_SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::MetalRoughSpheres:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/MetalRoughSpheres/glTF/MetalRoughSpheres.gltf");
          m_SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;


        case ExampleScene::MetalRoughSpheresNoTextures:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/MetalRoughSpheresNoTextures/glTF/MetalRoughSpheresNoTextures.gltf");
          m_SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::FlightHelmet:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/FlightHelmet/glTF/FlightHelmet.gltf");
          m_SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::Cube:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/Cube/glTF/Cube.gltf");
          m_SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::Box:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/Box/glTF/Box.gltf");
          m_SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::Buggy:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/Buggy/glTF/Buggy.gltf");
          m_SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
          break;

        case ExampleScene::Sponza:

          m_Scenes = m_SceneLoader.LoadGLTF(device, "vendor/gltf-Sample-Models/2.0/Sponza/glTF/Sponza.gltf");
          m_SelectedSceneIndex = m_Scenes.size() > 0 ? 0 : -1;
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
                m_Viewport.GetWidth(),
                m_Viewport.GetHeight(),
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

        return true;

      }
    );

    eventLib.Publish< ExampleSceneLoadEvent>({ ExampleScene::Duck });

    PSTC_INFO("{0} attached!", GetName());
  }

  void EditorLayer::OnDetach(Window& window, EventLibrary&)
  {
    PSTC_INFO("{0} detached!", GetName());
  }

  void EditorLayer::OnUpdate(std::chrono::duration<float> dt)
  {

    uint32_t viewportWidth = m_Viewport.GetWidth();
    uint32_t viewportHeight = m_Viewport.GetHeight();

    Camera& camera = GetActiveCamera();
    CameraOrbitController& cameraOrbitController = GetActiveCameraController();

    cameraOrbitController.UpdateCamera(camera, dt);

    glm::uvec2 viewportDimensions = camera.GetViewportDimensions();

    if (viewportDimensions.x != viewportWidth || viewportDimensions.y != viewportHeight)
      camera.UpdateProjection(fovY, viewportWidth, viewportHeight, zNear, zFar);

  }

  void EditorLayer::OnRender(Device& device, Window& window)
  {
    if (m_SelectedSceneIndex < 0 || m_Scenes.size() < m_SelectedSceneIndex)
      return;

    Camera& camera = GetActiveCamera();
    glm::uvec2 viewportDimensions = camera.GetViewportDimensions();

    if (m_Scenes.size() > 0)
      m_SceneRenderer->Render(device, m_Scenes[m_SelectedSceneIndex], camera, viewportDimensions.x, viewportDimensions.y, m_ClearColor);
  }

  void EditorLayer::OnGuiRender(Window& window, EventLibrary& eventLibrary)
  {
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    m_Dockspace->SetPanelDockingInfo({ "Dockspace", m_PerformanceTracker.GetName() , ImGuiDir_Left });
    m_Dockspace->SetPanelDockingInfo({ "Dockspace", "Temp Debug Panel" , ImGuiDir_Left });
    //m_Dockspace.SetPanelDockingInfo({ m_PerformanceTracker.GetName(), "Temp Debug Panel", ImGuiDir_Down }); // Todo: make this line work and delete previous line
    m_Dockspace->Render(eventLibrary);
    m_PerformanceTracker.Render(window, m_ClearColor);


    ImGui::Begin("Temp Debug Panel");


    std::map<std::string, Ref<Attachment>> sceneAttachments = m_SceneRenderer->GetDisplayReadyAttachments();
    std::vector<const char*> attachmentNames;
    for (const auto& [name, att] : sceneAttachments)
      attachmentNames.push_back(name.c_str());

    static int selectedPresentTextureIndex = 0;
    ImGui::ListBox("Display texture", &selectedPresentTextureIndex, attachmentNames.data(), attachmentNames.size(), 3);


    std::vector<const char*> sceneNames;
    for (const auto& scene : m_Scenes)
      sceneNames.push_back(scene.GetSceneName().c_str());

    ImGui::ListBox("Scenes", &m_SelectedSceneIndex, sceneNames.data(), sceneNames.size(), 3);


    m_SelectedSceneCameraEntityMap.clear();
    std::vector<const char*> cameraNames;
    cameraNames.push_back("Editor Camera");
    if (m_SelectedSceneIndex >= 0 && m_SelectedSceneIndex < m_Scenes.size())
    {
      int i = 1;
      auto sceneCameraView = m_Scenes[m_SelectedSceneIndex].GetView<SemanticNameComponent, Camera>();
      for (const EntityID& id : sceneCameraView)
      {
        SemanticNameComponent& name = sceneCameraView.get<SemanticNameComponent>(id);
        m_SelectedSceneCameraEntityMap[i] = id;
        cameraNames.push_back(std::move(name.c_str()));
        i++;
      }
    }

    ImGui::ListBox("Cameras", &m_SelectedSceneCameraIndex, cameraNames.data(), cameraNames.size(), 3);

    Camera& camera = GetActiveCamera();
    glm::vec3 cameraPosition = camera.GetPosition();

    ImGui::Text("Primary camera location: (%.2f, %.2f, %.2f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);

    ImGui::End();

    if (attachmentNames.size() > 0)
    {
      std::string texName = std::string(attachmentNames[selectedPresentTextureIndex]);
      Ref<Attachment> activeDisplayAttachment = sceneAttachments[texName];
      m_Viewport.Render(window, GetActiveCameraController(), activeDisplayAttachment);
    }


  }

}



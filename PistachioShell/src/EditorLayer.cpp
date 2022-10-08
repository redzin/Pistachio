#include "EditorLayer.h"

namespace Pistachio
{

  static void LoadSceneOneMillionSprites()
  {

  }

  void EditorLayer::OnAttach(Window& window, EventLibrary& eventLib)
  {
    Device& device = window.GetDevice();
    m_SceneRenderer = CreateScope<SceneRenderer>(device);
    m_Dockspace = CreateScope<MainDockspace>(eventLib);

    eventLib.Subscribe<KeyEvent>([&eventLib](KeyEvent e)
      {
        if (e.key == Input::KeyCode::Escape && e.action == Input::ButtonAction::KeyPressed)
          eventLib.Publish<WindowCloseEvent>(WindowCloseEvent{});
        return true;
      }
    );

    eventLib.Subscribe<ExampleSceneLoadEvent>([&eventLib, &device, this](ExampleSceneLoadEvent e)
      {

        m_Scene.Clear();

        switch (e.ExampleScene)
        {
        case ExampleScene::None:

          break;
        case ExampleScene::Box:
          break;
        case ExampleScene::OneMillionSprites:

          auto texChechkerboardPath = "assets/textures/Checkerboard.png";
          auto texChernoPath = "assets/textures/ChernoLogo.png";
          auto texCherno2Path = "assets/textures/ChernoLogo2.png";
          std::unordered_map<std::string, uint32_t> textureMap = { {texChernoPath, 0}, {texCherno2Path, 1} };

          int max = glm::pow(2, 10);
          float shift = max / 2.0;

          m_SceneRenderer->InitSpriteRenderer(device, textureMap, glm::pow(max, 2), 1024, 1024);

          SceneEntity spriteChernos2 = m_Scene.CreateEntity();
          spriteChernos2.AddComponent<SemanticNameComponent>("Chernos2");
          spriteChernos2.AddComponent<RenderableSpriteComponent>(device, glm::pow(max, 2));

          RenderableSpriteComponent& spriteComponent2 = spriteChernos2.GetComponent<RenderableSpriteComponent>();
          glm::mat4* transforms2 = (glm::mat4*)spriteComponent2.TransformBuffer->MemoryPtr;
          uint32_t* samplerIndices2 = (uint32_t*)spriteComponent2.TexCoordIndexBuffer->MemoryPtr;
          
          Timer t;
          for (int i = 0; i < max; i++)
          {
            for (int j = 0; j < max; j++)
            {
              transforms2[i + j * max] = glm::translate(glm::mat4(1.0f), glm::vec3(i - shift, j - shift, -0.5f));
              samplerIndices2[i + j * max] = textureMap[texCherno2Path];
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

    // Load Default
    eventLib.Publish< ExampleSceneLoadEvent>({ ExampleScene::OneMillionSprites });


    //LoadScene(); // wip

    PSTC_INFO("SampleTriangleLayer attached!");
  }

  void EditorLayer::OnDetach(Window& window, EventLibrary&)
  {
    PSTC_INFO("SampleTriangleLayer detached!");
  }

  void EditorLayer::OnUpdate(std::chrono::duration<float> dt)
  {

    camera = CameraController_Pan::UpdateCamera(camera, dt, cameraControllerPan);
    cameraControllerPan = CameraController_Pan::UpdateState(dt, cameraControllerPan);
    CameraController_Orbit::UpdateCamera(camera, dt, cameraControllerOrbit);

  }

  void EditorLayer::OnRender(Device& device, Window& window)
  {
    uint32_t viewportWidth = m_Viewport.GetWidth();
    uint32_t viewportHeight = m_Viewport.GetHeight();
    if (camera.viewport.x != viewportWidth || camera.viewport.y != viewportHeight)
    {
      camera = PerspectiveCamera::NewPerspective(camera, fovY, viewportWidth, viewportHeight, zNear, zFar);
    }

    m_SceneRenderer->Render(device, m_Scene, camera, viewportWidth, viewportHeight, m_ClearColor);

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


    std::map<std::string, Ref<Attachment>> sceneAttachments = m_SceneRenderer->GetDisplayReadyAttachments();
    std::vector<const char*> attachmentNames;
    for (const auto& [name, att] : sceneAttachments)
      attachmentNames.push_back(name.c_str());

    ImGui::Begin("Temp Debug Panel");
    static int selectedPresentTextureIndex = 0;
    ImGui::ListBox("Display texture", &selectedPresentTextureIndex, attachmentNames.data(), attachmentNames.size(), 3);
    ImGui::Text("Primary camera location: (%.2f, %.2f, %.2f)", camera.position.x, camera.position.y, camera.position.z);
    ImGui::End();

    if (attachmentNames.size() > 0)
    {
      std::string texName = std::string(attachmentNames[selectedPresentTextureIndex]);
      Ref<Attachment> activeDisplayAttachment = sceneAttachments[texName];
      m_Viewport.Render(window, cameraControllerOrbit, activeDisplayAttachment);
    }


  }

}



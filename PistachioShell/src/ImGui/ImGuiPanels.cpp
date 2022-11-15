#include "ImGuiPanels.h"

namespace Pistachio
{

  void Render(ClearColorPicker& performanceTracker)
  {
    ImGui::ColorEdit3("Clear color", (float*)&performanceTracker.ImguiClearColor);
    performanceTracker.ClearColor.x = performanceTracker.ImguiClearColor.x;
    performanceTracker.ClearColor.y = performanceTracker.ImguiClearColor.y;
    performanceTracker.ClearColor.z = performanceTracker.ImguiClearColor.z;
    performanceTracker.ClearColor.w = performanceTracker.ImguiClearColor.w;
  }

  void Render(PerformanceTracker& performanceTracker, Window& window)
  {
    ImGui::Begin(performanceTracker.Name.c_str());

    ImGui::Checkbox("VSync", &performanceTracker.VSync);

    if (performanceTracker.VSync != window.IsVSync())
      window.SetVSync(performanceTracker.VSync);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }

  void Render(RenderGraphAttachmentPanel& renderGraphAttachmentPanel, std::map<std::string, Ref<Attachment>> attachmentMap)
  {

    ImGui::Begin(renderGraphAttachmentPanel.Name.c_str());

    renderGraphAttachmentPanel.Attachments = attachmentMap;

    renderGraphAttachmentPanel.AttachmentNames.clear();
    for (const auto& [name, att] : renderGraphAttachmentPanel.Attachments)
      renderGraphAttachmentPanel.AttachmentNames.push_back(name.c_str());

    ImGui::ListBox("Display texture", &renderGraphAttachmentPanel.SelectedAttachmentIndex, renderGraphAttachmentPanel.AttachmentNames.data(), renderGraphAttachmentPanel.AttachmentNames.size(), 3);

    ImGui::End();
  }

  void Render(ScenePanel& scenePanel, CameraPanel& cameraPanel, std::vector<Scene>& scenes)
  {

    ImGui::Begin(scenePanel.Name.c_str());

    std::vector<const char*> sceneNames;
    for (const auto& scene : scenes)
      sceneNames.push_back(scene.GetSceneName().c_str());

    //ImGui::ListBox("Scenes", &scenePanel.SelectedSceneIndex, sceneNames.data(), sceneNames.size(), 3);

    if (sceneNames.size() > 0 && scenePanel.SelectedSceneIndex >= 0)
    {
      if (ImGui::BeginCombo("Scenes", sceneNames[scenePanel.SelectedSceneIndex]))
      {
        for (int n = 0; n < sceneNames.size(); n++)
        {
          const bool is_selected = (scenePanel.SelectedSceneIndex == n);
          if (ImGui::Selectable(sceneNames[n], is_selected))
            scenePanel.SelectedSceneIndex = n;

          // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
    }

    Render(cameraPanel, scenePanel, scenes);

    ImGui::End();

  }

  Camera& GetActiveCamera(CameraPanel& cameraPanel, Scene& scene)
  {
    auto editorCameraView = scene.GetView<Camera, EditorCameraTag>();
    if (cameraPanel.SelectedSceneCameraIndex < 1)
    {
      for (const EntityID& id : editorCameraView)
      {
        return editorCameraView.get<Camera>(id);
      }
    }

    EntityID selectedCameraID = cameraPanel.SelectedSceneCameraEntityMap[cameraPanel.SelectedSceneCameraIndex];
    auto sceneCameraView = scene.GetView<Camera>();
    for (const EntityID& id : sceneCameraView)
    {
      if (id == selectedCameraID)
        return sceneCameraView.get<Camera>(id);
    }

    for (const EntityID& id : editorCameraView)
    {
      return editorCameraView.get<Camera>(id);
    }

    PSTC_ASSERT(false, "No editor camera found!");
  }

  CameraOrbitController& GetActiveCameraController(CameraPanel& cameraPanel, Scene& scene)
  {
    auto editorCameraControllerView = scene.GetView<CameraOrbitController, EditorCameraTag>();
    if (cameraPanel.SelectedSceneCameraIndex < 1)
    {
      for (const EntityID& id : editorCameraControllerView)
      {
        return editorCameraControllerView.get<CameraOrbitController>(id);
      }
    }

    EntityID selectedCameraID = cameraPanel.SelectedSceneCameraEntityMap[cameraPanel.SelectedSceneCameraIndex];
    auto sceneCameraView = scene.GetView<CameraOrbitController>();
    for (const EntityID& id : sceneCameraView)
    {
      if (id == selectedCameraID)
        return sceneCameraView.get<CameraOrbitController>(id);
    }

    for (const EntityID& id : editorCameraControllerView)
    {
      return editorCameraControllerView.get<CameraOrbitController>(id);
    }

    PSTC_ASSERT(false, "No editor camera controller found!");
  }

  void Render(CameraPanel& cameraPanel, ScenePanel& scenePanel, std::vector<Scene>& scenes)
  {
    //ImGui::Begin(cameraPanel.Name.c_str());

    cameraPanel.SelectedSceneCameraEntityMap.clear();
    std::vector<const char*> cameraNames;
    //cameraNames.push_back("Editor Camera");
    if (scenePanel.SelectedSceneIndex >= 0 && scenePanel.SelectedSceneIndex < scenes.size())
    {
      int i = 1;
      auto sceneCameraView = scenes[scenePanel.SelectedSceneIndex].GetView<SemanticNameComponent, Camera>();
      for (const EntityID& id : sceneCameraView)
      {
        SemanticNameComponent& name = sceneCameraView.get<SemanticNameComponent>(id);
        cameraPanel.SelectedSceneCameraEntityMap[i] = id;
        cameraNames.push_back(std::move(name.c_str()));
        i++;
      }
    }

    ImGui::ListBox("Cameras", &cameraPanel.SelectedSceneCameraIndex, cameraNames.data(), cameraNames.size(), 3);

    if (scenePanel.SelectedSceneIndex >= 0 && scenePanel.SelectedSceneIndex < scenes.size())
    {
      glm::vec3 cameraPosition = GetActiveCamera(cameraPanel, scenes[scenePanel.SelectedSceneIndex]).GetPosition();
      ImGui::Text("Primary camera location: (%.2f, %.2f, %.2f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    }

    //ImGui::End();
  }

  bool OnViewportMouseClickEvent(Viewport& viewport, ViewportMouseClickEvent& e, Window& window, CameraOrbitController& cameraOrbitController)
  {
    if (e.action == Input::ButtonAction::KeyPressed && e.mouseKey == Input::MouseCode::ButtonLeft)
    {
      cameraOrbitController.EnableOrbiting(e.x, e.y);
      window.HideCursor();
    }

    if (e.action == Input::ButtonAction::KeyReleased && e.mouseKey == Input::MouseCode::ButtonLeft)
    {
      cameraOrbitController.DisableOrbiting(e.x, e.y);
      window.ShowCursor();
    }

    if (e.action == Input::ButtonAction::KeyPressed && e.mouseKey == Input::MouseCode::ButtonRight)
    {
      cameraOrbitController.EnablePanning(e.x, e.y);
      window.HideCursor();
    }

    if (e.action == Input::ButtonAction::KeyReleased && e.mouseKey == Input::MouseCode::ButtonRight)
    {
      cameraOrbitController.DisablePanning(e.x, e.y);
      window.ShowCursor();
    }

    return true;
  }

  bool OnViewportMouseMoveEvent(Viewport& viewport, ViewportMouseMoveEvent& e, CameraOrbitController& cameraOrbitController)
  {
    cameraOrbitController.OnMouseMoveEvent(e);
    return true;
  }

  bool OnViewportScrollEvent(Viewport& viewport, ViewportMouseScrollEvent& e, CameraOrbitController& cameraOrbitController)
  {
    cameraOrbitController.OnMouseScrollEvent(e);
    return true;
  }

  void Render(Viewport& viewport, RenderGraphAttachmentPanel& attachmentPanel, ScenePanel& scenePanel, Window& window, CameraOrbitController& cameraControllerOrbit)
  {

    if (attachmentPanel.AttachmentNames.size() < 1 || scenePanel.SelectedSceneIndex < 0)
      return;

    std::string texName = std::string(attachmentPanel.AttachmentNames[attachmentPanel.SelectedAttachmentIndex]);
    Ref<Attachment> presentTexture = attachmentPanel.Attachments[texName];

    ImGuiWindowFlags viewportWindowFlags = 0;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin(viewport.Name.c_str(), nullptr, viewportWindowFlags);
    //ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImVec2 viewportPanelSize = ImGui::GetWindowSize();
    viewport.Width = viewportPanelSize.x;
    viewport.Height = viewportPanelSize.y;

    if (presentTexture > 0)
      ImGui::Image((void*)presentTexture->RendererID, viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::IsWindowHovered() && ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
      ViewportMouseClickEvent e
      {
        Input::ButtonAction::KeyPressed,
        Input::MouseCode::ButtonLeft,
        0,
        window.GetMouseX(),
        window.GetMouseY()
      };
      OnViewportMouseClickEvent(viewport, e, window, cameraControllerOrbit);
    }

    if (ImGui::IsWindowHovered() && ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
    {
      ViewportMouseClickEvent e
      {
        Input::ButtonAction::KeyPressed,
        Input::MouseCode::ButtonRight,
        0,
        window.GetMouseX(),
        window.GetMouseY()
      };
      OnViewportMouseClickEvent(viewport, e, window, cameraControllerOrbit);
    }

    ViewportMouseScrollEvent e = { window.GetScrollX(), window.GetScrollY() };
    if (ImGui::IsWindowHovered() && ImGui::IsItemHovered() && std::abs(e.yoffset) > 0.1)
    {
      OnViewportScrollEvent(viewport, e, cameraControllerOrbit);
      ImGui::SetItemUsingMouseWheel();
    }

    ImGui::End();
    ImGui::PopStyleVar();

    ViewportMouseMoveEvent posEvent{ window.GetMouseX(), window.GetMouseY() };
    if ((cameraControllerOrbit.IsOrbitingEnabled() && ImGui::IsMouseDown(0)) || (cameraControllerOrbit.IsPanningEnabled() && ImGui::IsMouseDown(1)))
    {
      OnViewportMouseMoveEvent(viewport, posEvent, cameraControllerOrbit);
    }

    if (cameraControllerOrbit.IsOrbitingEnabled() && !ImGui::IsMouseDown(0))
    {
      ViewportMouseClickEvent e
      {
        Input::ButtonAction::KeyReleased,
        Input::MouseCode::ButtonLeft,
        0,
        posEvent.x,
        posEvent.y
      };
      OnViewportMouseClickEvent(viewport, e, window, cameraControllerOrbit);
    }

    if (cameraControllerOrbit.IsPanningEnabled() && !ImGui::IsMouseDown(1))
    {
      ViewportMouseClickEvent e
      {
        Input::ButtonAction::KeyReleased,
        Input::MouseCode::ButtonRight,
        0,
        posEvent.x,
        posEvent.y
      };
      OnViewportMouseClickEvent(viewport, e, window, cameraControllerOrbit);
    }

  }

  void Render(PBRShaderPanel& pbrShaderPanel)
  {
    ImGui::Begin(pbrShaderPanel.Name.c_str());

    ImGui::Checkbox("Normal Mapping", &pbrShaderPanel.PBRShaderOverrides.NormalMappingEnabled);

    Render(pbrShaderPanel.ClearColorPicker);

    ImGui::End();
  }

  void Render(MainDockspace& dockspace, EventLibrary& eventLibrary)
  {
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGuiDockNodeFlags dockspace_flags
      = ImGuiDockNodeFlags_NoDockingInCentralNode;
    //| ImGuiDockNodeFlags_PassthruCentralNode;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
  // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    bool open = true;
    ImGui::Begin("Dockspace", &open, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    // Submit the DockSpace
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("Dockspace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

      static auto first_time = true;
      if (first_time)
      {
        first_time = false;

        ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
        ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
        //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
        //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
        
        auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.3f, nullptr, &dockspace_id);
        auto dock_id_left2 = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.80f, nullptr, &dock_id_left);
        auto dock_id_left3 = ImGui::DockBuilderSplitNode(dock_id_left2, ImGuiDir_Down, 0.750f, nullptr, &dock_id_left2);

        ImGui::DockBuilderDockWindow(dockspace.RenderViewport.Name.c_str(), dockspace_id);
        ImGui::DockBuilderDockWindow(dockspace.PerformanceTracker.Name.c_str(), dock_id_left);
        ImGui::DockBuilderDockWindow(dockspace.PBRShaderPanel.Name.c_str(), dock_id_left2);
        ImGui::DockBuilderDockWindow(dockspace.ScenePanel.Name.c_str(), dock_id_left3);
        ImGui::DockBuilderDockWindow(dockspace.RenderGraphAttachmentPanel.Name.c_str(), dock_id_left3);
        ImGui::DockBuilderFinish(dockspace_id);
        ImGuiDockNode* Node = ImGui::DockBuilderGetNode(dockspace_id);
        Node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar;

      }
    }

    if (ImGui::BeginMenuBar())
    {

      if (ImGui::BeginMenu("Scene"))
      {
        if (ImGui::BeginMenu("Examples"))
        {

          if (ImGui::MenuItem("Empty Scene"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::None });
          }

          if (ImGui::MenuItem("One Million Sprites"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::OneMillionSprites });
          }

          if (ImGui::MenuItem("Cube"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::Cube });
          }

          if (ImGui::MenuItem("Box"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::Box });
          }

          if (ImGui::MenuItem("BoxWithVertexColors"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::BoxVertexColors });
          }

          if (ImGui::MenuItem("Duck"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::Duck });
          }

          if (ImGui::MenuItem("MetalRoughSpheres"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::MetalRoughSpheres });
          }

          if (ImGui::MenuItem("EnvironmentTest"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::EnvironmentTest });
          }

          if (ImGui::MenuItem("FlightHelmet"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::FlightHelmet });
          }

          if (ImGui::MenuItem("Buggy"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::Buggy });
          }

          if (ImGui::MenuItem("ABeautifulGame"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::ABeautifulGame });
          }

          if (ImGui::MenuItem("Sponza"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::Sponza });
          }

          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Options"))
      {
        if (ImGui::MenuItem("Play", NULL, false, &open != NULL))
        {
          //Enter play mode
        };
        ImGui::Separator();
        if (ImGui::MenuItem("Close", NULL, false, &open != NULL))
          eventLibrary.Publish<WindowCloseEvent>(std::move(WindowCloseEvent()));
        ImGui::EndMenu();
      }
      //ImGuiRenderer::HelpMarker("Helper text!");

      ImGui::EndMenuBar();
    }

    ImGui::End();
  }

}


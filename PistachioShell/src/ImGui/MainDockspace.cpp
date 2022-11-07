#include "MainDockspace.h"

namespace Pistachio
{

  MainDockspace::MainDockspace(EventLibrary& eventLibrary)
  {
    eventLibrary.Register<ExampleSceneLoadEvent>();
  }

  void Pistachio::MainDockspace::Render(EventLibrary& eventLibrary)
  {
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
    ImGuiIO& io = ImGui::GetIO();
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

        ImGui::DockBuilderDockWindow("Viewport", dockspace_id);
        ImGui::DockBuilderFinish(dockspace_id);
        ImGuiDockNode* Node = ImGui::DockBuilderGetNode(dockspace_id);
        Node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar;

        for (const auto& [hash, infos] : m_InitPanelLayout)
        {
          if (infos.size() < 1)
            continue;

          ImGuiID parent_id = ImGui::GetID(infos[0].Parent.c_str());
          ImGui::DockSpace(parent_id, ImVec2(1.0f, 1.0f), dockspace_flags);
          ImGui::DockBuilderSetNodeSize(parent_id, viewport->Size);
          auto dock_direction_id = ImGui::DockBuilderSplitNode(parent_id, infos[0].Direction, 0.3f, nullptr, &parent_id);
          for (const auto& info : infos)
            ImGui::DockBuilderDockWindow(info.Child.c_str(), dock_direction_id);
        }

        //auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.3f, nullptr, &dockspace_id);
        //ImGui::DockBuilderDockWindow("Pistachio Control Panel", dock_id_left);
        //ImGui::DockBuilderDockWindow("Temp Debug Panel", dock_id_left);


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

          if (ImGui::MenuItem("MetalRoughSphere"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::MetalRoughSpheres});
          }

          if (ImGui::MenuItem("Duck"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::Duck });
          }

          if (ImGui::MenuItem("Cube"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::Cube });
          }

          if (ImGui::MenuItem("Box"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::Box });
          }

          if (ImGui::MenuItem("FlightHelmet"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::FlightHelmet });
          }

          if (ImGui::MenuItem("Buggy"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::Buggy });
          }

          if (ImGui::MenuItem("Sponza"))
          {
            eventLibrary.Publish<ExampleSceneLoadEvent>({ ExampleScene::Buggy });
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
    m_InitPanelLayout.clear();
  }

  void MainDockspace::SetPanelDockingInfo(PanelDockingInfo info)
  {
    Hasher hasher;
    for (const auto& c : info.Parent)
      hasher.hash(c);
    hasher.hash(info.Direction);
    m_InitPanelLayout[hasher.get()].push_back(info);
  }

}


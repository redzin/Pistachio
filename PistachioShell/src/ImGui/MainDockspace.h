#pragma once
#include <Pistachio.h>

namespace Pistachio
{

  enum class ExampleScene
  {
    None = 0,
    OneMillionSprites = 1,
    Box = 2
  };

  struct ExampleSceneLoadEvent
  {
    ExampleScene ExampleScene;
  };

  struct PanelDockingInfo
  {
    std::string Parent;
    std::string Child;
    ImGuiDir_ Direction;
    ImVec2 ParentSize;
  };

  class MainDockspace
  {
  public:
    MainDockspace(EventLibrary& eventLibrary);
    void Render(EventLibrary& eventLibrary);
    void SetPanelDockingInfo(PanelDockingInfo info);
  private:
    std::map<Hash, std::vector<PanelDockingInfo>> m_InitPanelLayout;
  };

}

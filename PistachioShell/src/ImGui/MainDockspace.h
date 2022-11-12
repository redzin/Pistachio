#pragma once
#include <Pistachio.h>

namespace Pistachio
{

  enum class ExampleScene
  {
    None = 0,
    OneMillionSprites,
    Duck,
    MetalRoughSpheres,
    MetalRoughSpheresNoTextures,
    FlightHelmet,
    Cube,
    Box,
    Buggy,
    Sponza
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


#pragma once
#include "pch.h"
#include "Pistachio/Rendering/RenderGraph.h"
namespace Pistachio
{
  class ImGuiRenderer
  {
  public:
    virtual ~ImGuiRenderer() = default;
    virtual void BeginRender() = 0;
    virtual void EndRender() = 0;

    static Scope<ImGuiRenderer> Create();

    // Helper to display a little (?) mark which shows a tooltip when hovered.
    // In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
    static void HelpMarker(const char* desc);

  };

}



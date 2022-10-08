#pragma once
#include "pch.h"

namespace Pistachio
{

  class Layer
  {
  public:
    ~Layer() = default;

    virtual void OnAttach(Window& window, EventLibrary& EventLib) { };
    virtual void OnDetach(Window& window, EventLibrary& EventLib) { };
    virtual void OnUpdate(std::chrono::duration<float> dt) { };
    virtual void OnRender(Device& device, Window& window) { };
    virtual void OnGuiRender(Window& window, EventLibrary& eventLibrary) { }

    virtual const char* GetName() const = 0;
  };



}


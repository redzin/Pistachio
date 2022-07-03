#pragma once
#include "pch.h"

namespace Pistacio
{

  class PSTC_API Layer
  {
  public:
    Layer(const std::string& debugName = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach() { }
    virtual void OnDetach() { }
    virtual void OnUpdate() { }
    virtual void OnGuiRender() { }
    virtual void OnEvent(Event& e) { }

    const std::string& GetName() const { return debugName; }

  private:
    const std::string debugName;

  };



}


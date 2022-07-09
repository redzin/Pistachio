#pragma once
#include "pch.h"

namespace Pistacio
{

  class Layer
  {
  public:
    ~Layer() = default;

    virtual void OnAttach() { };
    virtual void OnDetach() { };
    virtual void OnRender() { };
    virtual void OnGuiRender() { }

    virtual const char* GetName() const = 0;
  };



}


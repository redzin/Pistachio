#pragma once

#include "pch.h"

namespace Pistacio
{

  class Swapchain
  {
  public:
    virtual ~Swapchain() = default;
    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;
  };

}




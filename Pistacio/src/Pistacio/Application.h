#pragma once
#include <stdio.h>
#include "Core.h"


namespace Pistacio {
  class PSTC_API Application
  {
  public:
    Application() {}
    ~Application() {}

    void Run();
  };

  Application* CreateApplication();
}


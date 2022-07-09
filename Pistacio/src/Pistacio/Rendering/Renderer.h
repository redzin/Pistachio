#pragma once

#include "RenderCommand.h"

namespace Pistacio
{

  class Renderer
  {
  public:
    static void BeginScene();
    static void EndScene();
    static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

  private:

  };

}


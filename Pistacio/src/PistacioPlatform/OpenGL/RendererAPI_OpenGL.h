#pragma once

#include "Pistacio/Rendering/Renderer.h"

namespace Pistacio
{

  class RendererAPI_OpenGL : public RendererAPI
  {
  public:

    virtual void SetClearColor(const glm::vec4& color) override;

    virtual void Clear() override;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

  };

}

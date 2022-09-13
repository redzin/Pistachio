#pragma once
#include "pch.h"

struct SemanticNameComponent
{
  std::string Name;
};

struct SpriteRenderableComponent
{
  std::string SamplerName;
  uint32_t SamplerIndex;
  glm::mat4 Transform;
};

struct TransparentRenderableComponent { };


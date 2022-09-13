#include "pch.h"
#include "Scene.h"

namespace Pistacio
{

  SceneEntity Scene::CreateEntity()
  {
    EntityID entity = static_cast<EntityID>(m_Registry.create());
    return SceneEntity(*this, std::move(entity));
  }

}


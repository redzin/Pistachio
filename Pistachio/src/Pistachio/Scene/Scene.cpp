#include "pch.h"
#include "Scene.h"

namespace Pistachio
{

  SceneEntity Scene::CreateEntity()
  {
    EntityID entity = static_cast<EntityID>(m_Registry.create());
    return SceneEntity(*this, std::move(entity));
  }

  void Scene::Clear()
  {
    m_Registry.clear();
  }

}

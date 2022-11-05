#include "pch.h"
#include "Scene.h"

namespace Pistachio
{
  SceneEntity Scene::CreateEntity()
  {
    EntityID entity = static_cast<EntityID>(m_Registry.create());
    return SceneEntity(*this, entity);
  }

  SceneEntity Scene::CreateEntity(SceneEntity& parentEntity)
  {
    EntityID entity = static_cast<EntityID>(m_Registry.create());
    return SceneEntity(*this, entity, (EntityID)parentEntity);
  }

  void Scene::Clear()
  {
    m_Registry.clear();
  }

}


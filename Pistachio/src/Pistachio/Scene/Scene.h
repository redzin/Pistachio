#pragma once
#include "pch.h"
#include "entt.hpp"

namespace Pistachio
{
  using EntityID = entt::entity;

  class SceneEntity;

  class Scene
  {
  public:
    SceneEntity CreateEntity();

    template<typename... Components>
    auto GetView()
    {
      return m_Registry.view<Components...>();
    }

    template<typename... Components>
    auto GetComponents(EntityID id)
    {
      return m_Registry.get<Components...>(id);
    }
    
  private:
    template<typename ...T>
    using GroupGet_t = entt::get_t<T...>;
    template<typename ...T>
    using GroupExclude_t = entt::exclude_t<T...>;
  public:
    template<typename ...T>
    static constexpr GroupGet_t<T...> GroupGet = entt::get<T...>;
    template<typename ...T>
    static constexpr GroupExclude_t<T...> GroupExclude = entt::exclude<T...>;

    template<typename... Owned, typename... Get, typename... Exclude>
    auto GetGroup(GroupGet_t<Get...> getter, GroupExclude_t<Exclude...> excluder = {})
    {
      return m_Registry.group<Owned...>(getter, excluder);
    }

    template<typename... Owned>
    auto GetGroup()
    {
      return m_Registry.group<Owned...>();
    }

    template<typename Component>
    void Sort(std::function<bool(const EntityID, const EntityID)> comparator)
    {
      m_Registry.sort<Component>(comparator);
    }

    template<typename... Components>
    void Sort()
    {
      m_Registry.sort<Components...>();
    }

    template<typename... Owned, typename... Exclude>
    auto GetGroup(entt::exclude_t<Exclude...> excluder = {})
    {
      return m_Registry.group<Owned...>(excluder);
    }

    void Clear();

  private:
    friend class SceneEntity;
    entt::registry m_Registry;
  };


  class SceneEntity
  {
  public:
    SceneEntity(Scene& scene, EntityID&& id) : m_EntityID(std::move(id)), m_Scene(scene) {}
    SceneEntity(Scene& scene, const EntityID&& id) : m_EntityID(id), m_Scene(scene) {}
    ~SceneEntity() = default;

    template<typename Component, typename... Args>
    void AddComponent(Args&&... args)
    {
      PSTC_CORE_ASSERT(!HasAnyOf<Component>(), "Component already exists on entity!");
      m_Scene.m_Registry.emplace<Component>(m_EntityID, std::forward<Args>(args)...);
    }

    template<typename Component, typename... Args>
    void AddOrReplaceComponent(Args &&...args)
    {
      m_Scene.m_Registry.emplace_or_replace<Component>(m_EntityID, std::forward<Args>(args)...);
    }

    template<typename T>
    T& GetComponent() const
    {
      T& component = m_Scene.m_Registry.get<T>(m_EntityID);
      return component;
    }

    template<typename... Components>
    bool HasAnyOf() const
    {
      return m_Scene.m_Registry.any_of<Components...>(m_EntityID);
    }

    template<typename... Components>
    bool HasAllOf() const
    {
      return m_Scene.m_Registry.all_of<Components...>(m_EntityID);
    }

    template<typename... Components>
    void RemoveComponent()
    {
      m_Scene.m_Registry.remove<Components...>(m_EntityID);
    }

    operator bool() const { return m_EntityID != entt::null; }
    operator entt::entity() const { return static_cast<entt::entity>(m_EntityID); }

  private:
    EntityID m_EntityID;
    Scene& m_Scene;
  };

}

#include "SceneLoader.h"
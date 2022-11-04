#pragma once
#include "pch.h"
#include "entt.hpp"

namespace Pistachio
{
  using EntityID = entt::entity;

  class SceneEntity;

  class Scene
  {

  private:
    friend class SceneEntity;
    entt::registry m_Registry;
    std::string m_Name;

  private:
    template<typename ...T>
    using GroupGet_t = entt::get_t<T...>;
    template<typename ...T>
    using GroupExclude_t = entt::exclude_t<T...>;

  public:
    Scene() : m_Name("Untitled scene") {};
    Scene(const std::string& name) : m_Name(name) {};

    SceneEntity CreateEntity();
    SceneEntity CreateEntity(SceneEntity& parentEntity);

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

    const std::string& GetSceneName() const { return m_Name; }

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

  };

  struct SceneEntityRelationship
  {
    EntityID ParentID = entt::null;
  };

  class SceneEntity
  {
  public:
    SceneEntity(const SceneEntity& other) : m_Scene(other.m_Scene), m_EntityID(other.m_EntityID) { }
    SceneEntity(Scene& scene) : m_Scene(scene), m_EntityID(entt::null) { } // null entity create by just passing scene
    ~SceneEntity() = default;
    SceneEntity operator=(const SceneEntity& other)
    {
      return SceneEntity(other.m_Scene, other.m_EntityID);
    }

    template<typename Component, typename... Args>
    decltype(auto) AddComponent(Args&&... args)
    {
      PSTC_CORE_ASSERT(!HasAnyOf<Component>(), "Component already exists on entity!");
      return m_Scene.m_Registry.emplace<Component>(m_EntityID, std::forward<Args>(args)...);
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

    bool HasParent()
    {
      if (!HasAllOf<SceneEntityRelationship>())
        return false;

      SceneEntityRelationship& parentRelationShip = GetComponent<SceneEntityRelationship>();
      return parentRelationShip.ParentID != entt::null;
    }

    SceneEntity GetParent()
    {
      SceneEntityRelationship& parentRelationShip = GetComponent<SceneEntityRelationship>();
      SceneEntity(m_Scene, parentRelationShip.ParentID);
    }

    operator bool() const { return m_EntityID != entt::null; }
    operator EntityID() const { return m_EntityID; }
    //operator entt::entity() const { return static_cast<entt::entity>(m_EntityID); }

  private:
    friend Scene;

    SceneEntity(Scene& scene, EntityID id) : m_Scene(scene), m_EntityID(id) { }
    SceneEntity(Scene& scene, EntityID id, EntityID parentId) : m_Scene(scene), m_EntityID(id)
    {
      if (parentId != entt::null)
      {
        SceneEntityRelationship relationship{ parentId };
        AddComponent<SceneEntityRelationship>(relationship);
      }

    }

    EntityID m_EntityID;
    Scene& m_Scene;
  };

}

#include "SceneLoader.h"
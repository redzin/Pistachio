#pragma once
#include "pch.h"

namespace Pistacio
{

  class EventPublisherBase
  {
  };

  class Subscription
  {
  public:
    Subscription(std::function<void()> callback) : callback(callback) { }
    ~Subscription() = default;

    inline void Unsubscribe()
    {
      callback();
    }

  private:
    std::function<void()> callback;
  };

  template <typename EventType>
  class EventPublisher : public EventPublisherBase {
  public:
    EventPublisher() : index(0), subscriptions(std::map<uint64_t, std::function<bool(EventType)>>()) { }
    ~EventPublisher() = default;
    inline void Publish(EventType& e)
    {
      bool shouldContinue = true;
      for (auto const& [index, f] : subscriptions)
      {
        if (!shouldContinue)
          break;
        shouldContinue = f(e);
      }
    }

    inline Subscription Subscribe(std::function<bool(EventType)> action)
    {
      uint64_t i = index;
      subscriptions.insert(std::make_pair(i, action));
      index++;
      return Subscription([this, i]() mutable { subscriptions.erase(i); });
    }

  private:
    uint64_t index;
    std::map<uint64_t, std::function<bool(EventType)>> subscriptions;
  };

  class EventLibrary
  {
  public:
    template<typename EventType>
    inline void Register()
    {
      std::type_index index = typeid(EventType);

      if (publishers.find(index) == publishers.end())
      {
        publishers.insert(std::pair<std::type_index, EventPublisherBase*>(index, new EventPublisher<EventType>));
      }

      PSTC_CORE_ASSERT(publishers[index] != nullptr, "Event registration failed!");
    }

    template<typename EventType>
    inline Subscription Subscribe(std::function<bool(EventType)> callback)
    {
      std::type_index index = typeid(EventType);
#ifdef PSTC_DEBUG
      bool isAllocated = publishers.find(index) != publishers.end();
      PSTC_CORE_ASSERT(isAllocated, "Unregistered event type: \"{0}\"! Use EventLibrary.Register<EventType>() to register an event type.", typeid(EventType).name());
#endif

      return reinterpret_cast<EventPublisher<EventType>*>(publishers[index])->Subscribe(callback);
    }

    template<typename EventType>
    inline void Publish(EventType e)
    {
      std::type_index index = typeid(EventType);
#ifdef PSTC_DEBUG
      bool isAllocated = publishers.find(index) != publishers.end();
      PSTC_CORE_ASSERT(isAllocated, "Unregistered event type: \"{0}\"! Use EventLibrary.Register<EventType>() to register an event type.", typeid(EventType).name());
#endif

      reinterpret_cast<EventPublisher<EventType>*>(publishers[index])->Publish(e);
    }

  private:
    std::unordered_map<std::type_index, EventPublisherBase*> publishers;
  };


}

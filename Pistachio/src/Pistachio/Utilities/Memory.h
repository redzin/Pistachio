#pragma once
#include <vector>
#include <memory>

namespace Pistachio
{
  template<typename T>
  using Scope = std::unique_ptr<T>;
  template<typename T, typename ... Args>
  constexpr Scope<T> CreateScope(Args&& ... args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template<typename T>
  using Ref = std::shared_ptr<T>;
  template<typename T, typename ... Args>
  constexpr Ref<T> CreateRef(Args&& ... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  //template<typename T>
  //class ObjectPool;
  //
  //template<typename T>
  //class PoolObjectPtr
  //{
  //  friend class ObjectPool<T>;
  //public:
  //  PoolObjectPtr(const PoolObjectPtr<T>& other) : m_Ptr(other.m_Ptr), m_Source(other.m_Source), m_RefCount(other.m_RefCount)
  //  {
  //    (*m_RefCount).fetch_add(1u, std::memory_order_seq_cst);
  //  }
  //
  //  ~PoolObjectPtr()
  //  {
  //    (*m_RefCount).fetch_sub(1u, std::memory_order_seq_cst);
  //    if (*m_RefCount == 0)
  //      m_Source->Free(m_Ptr);
  //  }
  //
  //  T& operator*()
  //  {
  //    return *m_Ptr;
  //  }
  //
  //  T* operator->()
  //  {
  //    return m_Ptr;
  //  }
  //
  //private:
  //  PoolObjectPtr(T* ptr, std::shared_ptr<ObjectPool<T>> source) : m_Ptr(ptr), m_Source(source), m_RefCount(new uint32_t(1u)) { }
  //  T* m_Ptr;
  //  std::shared_ptr<ObjectPool<T>> m_Source;
  //  std::atomic<uint32_t>* m_RefCount; // cppcon talk on atomics https://www.youtube.com/watch?v=ZQFzMfHIxng
  //  
  //};


  template<typename T>
  class ObjectPool
  {
    //friend class ObjectPoolPtr<T>;
  public:
    template <typename... Args>
    T* Allocate(Args&& ...args)
    {
      if (vacants.empty())
      {
        uint32_t numObjectsInNewBlock = 64u << memory.size(); // Default size is 64 objects. Bitshift to double it for each block already alocated
        T* memoryBegin = (T*)_aligned_malloc(numObjectsInNewBlock * sizeof(T), std::max((size_t)64u, alignof(T))); // _aligned_malloc is MSVC only! Todo: make system independent aligned_malloc function
        
        if (!memoryBegin)
          return nullptr;

        for (uint32_t i = 0; i < numObjectsInNewBlock; i++)
          vacants.push_back(&memoryBegin[i]);

        memory.emplace_back(memoryBegin);
      }

      T* ptr = vacants.back();
      vacants.pop_back();
      new(ptr) T(std::forward<Args>(args)...);
      return ptr;
    }

    void Clear()
    {
      vacants.clear();
      memory.clear();
    }

    void Free(T* ptr) // <- This function can cause fragmentation
    {
      ptr->~T();
      vacants.push_back(ptr);
    }

  private:

    struct AlignedMallocDeleter
    {
      void operator()(T* ptr)
      {
        ::_aligned_free(ptr); // MSVC only! Todo: add platform independent version of this function
      }
    };

    std::vector<std::unique_ptr<T, AlignedMallocDeleter>> memory; // holds pointers to each aligned block
    std::vector<T*> vacants; // holds list of pointers to all unassigned slots on the current allocated memory
  };


  template <typename S, typename T, uint32_t LIFETIME>
  class TemporaryHashMap
  {
  public:

    void Advance()
    {
      std::vector<S> keysToDelete;
      for (auto& [k, v] : m_Tracker)
      {
        if (v < LIFETIME)
        {
          m_Tracker[k]++;
        }
        else
        {
          keysToDelete.push_back(k);
        }
      }

      for (const auto& k : keysToDelete)
        Erase(k);
    }

    T& operator[](S key)
    {
      if (!Exists(key))
      {
        m_Values[key] = m_Pool.Allocate();
      }

      m_Tracker[key] = 0;
      return *m_Values[key];
    }

    template <typename... Args>
    void Emplace(S key, Args&& ...args)
    {
      m_Tracker[key] = 0;
      m_Values[key] = m_Pool.Allocate(std::forward<Args>(args)...);
    }

    bool Exists(S key)
    {
      return m_Values.find(key) != m_Values.end();
    }

    void Erase(S key)
    {
      if (!Exists(key))
        return;

      m_Pool.Free(m_Values[key]);
      m_Values.erase(key);
      m_Tracker.erase(key);
    }

  private:

    ObjectPool<T> m_Pool;
    std::unordered_map<S, T*> m_Values;
    std::unordered_map<S, uint32_t> m_Tracker;

  };




}


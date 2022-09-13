#pragma once
#include <vector>
#include <memory>

namespace Pistacio
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
  //class ObjectPoolPtr
  //{
  //  friend class ObjectPool<T>;
  //public:
  //  ObjectPoolPtr(const ObjectPoolPtr<T>& other) : m_Ptr(other.m_Ptr), m_Source(other.m_Source)
  //  {
  //    RefCounter[&m_ptr]++;
  //  }
  //
  //  ~ObjectPoolPtr()
  //  {
  //    uint32_t count = RefCounter[&m_ptr]--;
  //    if (count == 0)
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
  //  ObjectPoolPtr(T* ptr, ObjectPool<T>* source) : m_Ptr(ptr), m_Source(source)
  //  {
  //    RefCounter[&m_ptr] = 1;
  //  }
  //  T* m_Ptr;
  //  ObjectPool<T>* m_Source;
  //
  //  volatile static std::unordered_map<size_t, uint32_t> RefCounter = {};
  //};

  // Is this class actually better for performance than std::vector? Todo: Test
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

    // Should be private and called by proper ref counting system
    void Free(T* ptr) // <- This function can cause fragmentation so use with care
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
}
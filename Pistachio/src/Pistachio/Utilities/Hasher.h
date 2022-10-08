#pragma once
#include <cstdint>
#include <string>

namespace Pistachio
{
  using Hash = uint64_t;
  class Hasher
  {
  public:
    Hasher() = default;
    Hasher(Hash seed) : h(seed) {}
    Hash get() { return h; }

    void hash(uint64_t v)
    {
      // https://stackoverflow.com/questions/5889238/why-is-xor-the-default-way-to-combine-hashes#:~:text=xor%20is%20a%20dangerous%20default,the%20same%20as%20%22dab%22%20.
      h ^= v + noise + (h << 6) + (v >> 2);
    }
    
    void data(const void* data, size_t Size)
    {
      if (Size % 8 == 0)
        for (size_t i = 0; i < Size; i += 8)
        {
          hash(((const uint64_t*)data)[i]);
        }
      else if (Size % 4 == 0)
        for (size_t i = 0; i < Size; i += 4)
        {
          hash(((const uint32_t*)data)[i]);
        }
      else if (Size % 2 == 0)
        for (size_t i = 0; i < Size; i += 2)
        {
          hash(((const uint16_t*)data)[i]);
        }
      else
        for (size_t i = 0; i < Size; i++)
        {
          hash(((const char*)data)[i]);
        }
    }

  private:
    Hash h = 0xcbf29ce484222325ull;
    uint64_t noise = 0xcbf29ce484222325ull;

  };
}

#pragma once
#include "pch.h"

namespace Pistacio
{

  static enum class BufferDataType { None = 0, Bool, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat3, Mat4 };

  inline static uint32_t ShaderDataTypeSize(BufferDataType& type)
  {
    {
      switch (type)
      {
      case BufferDataType::Bool:    return 1;
      case BufferDataType::Int:     return 4;
      case BufferDataType::Int2:    return 4 * 2;
      case BufferDataType::Int3:    return 4 * 3;
      case BufferDataType::Int4:    return 4 * 4;
      case BufferDataType::Float:   return 4;
      case BufferDataType::Float2:  return 4 * 2;
      case BufferDataType::Float3:  return 4 * 3;
      case BufferDataType::Float4:  return 4 * 4;
      case BufferDataType::Mat3:    return 4 * 3 * 3;
      case BufferDataType::Mat4:    return 4 * 4 * 4;
      }

      PSTC_CORE_ASSERT(false, "Undefined shader data type!");
      return 0;
    }
  }

  

  struct BufferEntry {
    BufferDataType type;
    std::string name;
    uint32_t offset;
    uint32_t size;
    bool normalized;

    BufferEntry(BufferDataType type, const std::string& name, bool normalized = false)
      : type(type), name(name), offset(0), size(ShaderDataTypeSize(type)), normalized(normalized)
    {
    }

    uint32_t GetComponentCount() const
    {
      switch (type)
      {
      case BufferDataType::Bool:    return 1;
      case BufferDataType::Int:     return 1;
      case BufferDataType::Int2:    return 2;
      case BufferDataType::Int3:    return 3;
      case BufferDataType::Int4:    return 4;
      case BufferDataType::Float:   return 1;
      case BufferDataType::Float2:  return 2;
      case BufferDataType::Float3:  return 3;
      case BufferDataType::Float4:  return 4;
      case BufferDataType::Mat3:    return 3 * 3;
      case BufferDataType::Mat4:    return 4 * 4;
      }

      PSTC_CORE_ASSERT(false, "Undefined shader data type!");
      return 0;
    }
  };

  class BufferLayout
  {
  public:

    BufferLayout() = default;
    ~BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferEntry>& elements) : entries(elements)
    {
      CalculateOffsetAndStride();
    }

    uint32_t GetStride() {
      return stride;
    }

    std::vector<BufferEntry>::iterator begin() { return entries.begin(); }
    std::vector<BufferEntry>::iterator end() { return entries.end(); }
    std::vector<BufferEntry>::reverse_iterator rbegin() { return entries.rbegin(); }
    std::vector<BufferEntry>::reverse_iterator rend() { return entries.rend(); }
    std::vector<BufferEntry>::const_iterator begin() const { return entries.begin(); }
    std::vector<BufferEntry>::const_iterator end() const { return entries.end(); }
    std::vector<BufferEntry>::const_reverse_iterator rbegin() const { return entries.rbegin(); }
    std::vector<BufferEntry>::const_reverse_iterator rend() const { return entries.rend(); }

  private:

    void CalculateOffsetAndStride()
    {
      uint32_t offset = 0;
      for (auto& entry : entries)
      {
        entry.offset = offset;
        offset += entry.size;
        stride += entry.size;
      }
    }

    std::vector<BufferEntry> entries;
    uint32_t stride = 0;
  };

  // Vertex Buffer //////////////////////////////////////////////////
  class VertexBuffer
  {
  public:
    virtual ~VertexBuffer() {}
    virtual const RendererID GetRendererID() const = 0;
    virtual void SetLayout(BufferLayout layout) = 0;
    virtual const BufferLayout& GetLayout() const = 0;

    static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
  };

  // Index Buffer //////////////////////////////////////////////////
  class IndexBuffer
  {
  public:
    virtual ~IndexBuffer() {}
    virtual const RendererID GetRendererID() const = 0;
    virtual uint32_t GetCount() const = 0;

    static Ref<IndexBuffer> Create(uint32_t* vertices, uint32_t size);
  };
}
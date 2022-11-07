#pragma once
#include "pch.h"

// TODO: Separate all enum definitions out into platform dependent file

namespace Pistachio
{

  class Device;
  using RendererID = uint32_t;


  /////////////////////////////////////////////////////////
  /// Capabilities
  /////////////////////////////////////////////////////////

  enum class DepthFunction
  {
    Never = 0x0200,
    Less = 0x0201,
    Equal = 0x0202,
    LEqual = 0x0203,
    Greater = 0x0204,
    NotEqual = 0x0205,
    GEqual = 0x0206,
    Always = 0x0207
  };

  enum class BlendFunction
  {
    Zero = 0,
    One = 1,
    SrcColor = 0x0300,
    OneMinusSrcColor = 0x0301,
    SrcAlpha = 0x0302,
    OneMinusSrcAlpha = 0x0303,
    DstAlpha = 0x0304,
    OneMinusDstAlpha = 0x0305,
    DstColor = 0x0306,
    OneMinusDstColor = 0x0307,
    SrcAlphaSaturate = 0x0308
  };

  enum class Capability
  {
    CullFace = 0x0B44,
    DepthTest = 0x0B71,
    ScissorTest = 0x0C11,
    StencilTest = 0x0B90,
    Blend = 0x0BE2,
    LineSmooth = 0x0B20
  };



  /////////////////////////////////////////////////////////
  /// Shaders
  /////////////////////////////////////////////////////////

  struct Shader
  {
    RendererID RendererID;
  };


  /////////////////////////////////////////////////////////
  /// Buffers
  /////////////////////////////////////////////////////////

  typedef enum class BufferDataType
  {
    None = 0,
    Bool,
    Byte,
    UnsignedByte,
    UnsignedByte2,
    Short,
    UnsignedShort,
    UnsignedShort2,
    Int,
    Int2,
    Int3,
    Int4,
    UnsignedInt,
    Float,
    Float2,
    Float3,
    Float4,
    Double,
    Mat3,
    Mat4
  };

  static uint32_t ShaderDataTypeToOpenGLBaseType(BufferDataType Type)
  {
    
    switch (Type)
    {
    case BufferDataType::Bool:            return 0x8B56; // GL_BOOL
    case BufferDataType::Byte:            return 0x1400; // GL_BYTE
    case BufferDataType::UnsignedByte:    return 0x1401; // GL_UNSIGNED_BYTE
    case BufferDataType::UnsignedByte2:   return 0x1401; // GL_UNSIGNED_BYTE
    case BufferDataType::Short:           return 0x1402; // GL_SHORT
    case BufferDataType::UnsignedShort:   return 0x1403; // GL_UNSIGNED_SHORT
    case BufferDataType::UnsignedShort2:  return 0x1403; // GL_UNSIGNED_SHORT
    case BufferDataType::Int:             return 0x1404; // GL_INT
    case BufferDataType::Int2:            return 0x1404; // GL_INT
    case BufferDataType::Int3:            return 0x1404; // GL_INT
    case BufferDataType::Int4:            return 0x1404; // GL_INT
    case BufferDataType::UnsignedInt:     return 0x1405; // GL_UNSIGNED_INT
    case BufferDataType::Float:           return 0x1406; // GL_FLOAT
    case BufferDataType::Float2:          return 0x1406; // GL_FLOAT
    case BufferDataType::Float3:          return 0x1406; // GL_FLOAT
    case BufferDataType::Float4:          return 0x1406; // GL_FLOAT
    case BufferDataType::Mat3:            return 0x1406; // GL_FLOAT
    case BufferDataType::Mat4:            return 0x1406; // GL_FLOAT
    }
    
    PSTC_ASSERT(false, "Unknown buffer data type!");
  }

  inline uint32_t ShaderDataTypeSize(const BufferDataType& Type)
  {
    {
      switch (Type)
      {
      case BufferDataType::Bool:            return 1;
      case BufferDataType::Byte:            return 1;
      case BufferDataType::UnsignedByte:    return 1;
      case BufferDataType::UnsignedByte2:   return 2;
      case BufferDataType::Short:           return 2;
      case BufferDataType::UnsignedShort:   return 2;
      case BufferDataType::UnsignedShort2:  return 2 * 2;
      case BufferDataType::Int:             return 4;
      case BufferDataType::Int2:            return 4 * 2;
      case BufferDataType::Int3:            return 4 * 3;
      case BufferDataType::Int4:            return 4 * 4;
      case BufferDataType::UnsignedInt:     return 4;
      case BufferDataType::Float:           return 4;
      case BufferDataType::Float2:          return 4 * 2;
      case BufferDataType::Float3:          return 4 * 3;
      case BufferDataType::Float4:          return 4 * 4;
      case BufferDataType::Mat4:            return 4 * 4 * 4;
      }

      PSTC_CORE_ASSERT(false, "Undefined shader data type!");
      return 0;
    }
  }

  inline uint32_t ShaderDataTypeComponentCount(const BufferDataType& Type)
  {
    switch (Type)
    {
    case BufferDataType::Bool:            return 1;
    case BufferDataType::Byte:            return 1;
    case BufferDataType::UnsignedByte:    return 1;
    case BufferDataType::UnsignedByte2:   return 2;
    case BufferDataType::Short:           return 1;
    case BufferDataType::UnsignedShort:   return 1;
    case BufferDataType::UnsignedShort2:  return 2;
    case BufferDataType::Int:             return 1;
    case BufferDataType::Int2:            return 2;
    case BufferDataType::Int3:            return 3;
    case BufferDataType::Int4:            return 4;
    case BufferDataType::Float:           return 1;
    case BufferDataType::Float2:          return 2;
    case BufferDataType::Float3:          return 3;
    case BufferDataType::Float4:          return 4;
    case BufferDataType::Mat4:            return 16;
    case BufferDataType::UnsignedInt:     return 1;
    }

    PSTC_CORE_ASSERT(false, "Undefined shader data type!");
    return 0;
  }

  inline uint32_t ShaderDataTypeSlotCount(const BufferDataType& Type)
  {
    switch (Type)
    {
    case BufferDataType::Bool:          return 1;
    case BufferDataType::Int:           return 1;
    case BufferDataType::Int2:          return 1;
    case BufferDataType::Int3:          return 1;
    case BufferDataType::Int4:          return 1;
    case BufferDataType::Float:         return 1;
    case BufferDataType::Float2:        return 1;
    case BufferDataType::Float3:        return 1;
    case BufferDataType::Float4:        return 1;
    case BufferDataType::Mat3:          return 3;
    case BufferDataType::Mat4:          return 4;
    case BufferDataType::UnsignedInt:   return 1;
    }

    PSTC_CORE_ASSERT(false, "Undefined shader data type!");
    return 0;
  }

  struct BufferLayoutEntry {
    BufferDataType Type;
    std::string SemanticName;
    uint32_t Offset;
    uint32_t Size;
    bool Normalized;

    BufferLayoutEntry(BufferDataType Type, const std::string& SemanticName, bool Normalized = false)
      : Type(Type), SemanticName(SemanticName), Offset(0), Size(ShaderDataTypeSize(Type)), Normalized(Normalized)
    {
    }
  };

  class BufferMemoryLayout
  {
  public:

    BufferMemoryLayout() = default;
    ~BufferMemoryLayout() = default;
    BufferMemoryLayout(const std::initializer_list<BufferLayoutEntry>& elements) : m_Entries(elements)
    {
      CalculateOffsetAndStride();
    }

    uint32_t GetStride() const {
      return m_Stride;
    }

    std::vector<BufferLayoutEntry>::iterator begin() { return m_Entries.begin(); }
    std::vector<BufferLayoutEntry>::iterator end() { return m_Entries.end(); }
    std::vector<BufferLayoutEntry>::reverse_iterator rbegin() { return m_Entries.rbegin(); }
    std::vector<BufferLayoutEntry>::reverse_iterator rend() { return m_Entries.rend(); }
    std::vector<BufferLayoutEntry>::const_iterator begin() const { return m_Entries.begin(); }
    std::vector<BufferLayoutEntry>::const_iterator end() const { return m_Entries.end(); }
    std::vector<BufferLayoutEntry>::const_reverse_iterator rbegin() const { return m_Entries.rbegin(); }
    std::vector<BufferLayoutEntry>::const_reverse_iterator rend() const { return m_Entries.rend(); }

  private:

    void CalculateOffsetAndStride()
    {
      uint32_t Offset = 0;
      for (auto& entry : m_Entries)
      {
        entry.Offset = Offset;
        Offset += entry.Size;
        m_Stride += entry.Size;
      }
    }

    std::vector<BufferLayoutEntry> m_Entries;
    uint32_t m_Stride = 0;
  };

  typedef enum
  {
    MAP_PERSISTENT_BIT = 0x0040,
    MAP_COHERENT_BIT = 0x0080,
    DYNAMIC_STORAGE_BIT = 0x0100,
    MAP_READ_BIT = 0x0001,
    MAP_WRITE_BIT = 0x0002,
    CLIENT_STORAGE_BIT = 0x0200
  } BufferFlagBits;
  using BufferFlag = uint32_t;

  struct BufferDescriptor
  {
    uint32_t Size = 0;
    BufferFlag Flags = MAP_WRITE_BIT | MAP_COHERENT_BIT | MAP_PERSISTENT_BIT;
    uint32_t AttributeDivisor = 0;
    BufferDataType DataType = BufferDataType::Float;
  };

  typedef enum
  {
    UNIFORM_BUFFER_TARGET = 0x8A11, //GL_UNIFORM_BUFFER,
    SHADER_STORAGE_BUFFER_TARGET = 0x90D2 // GL_SHADER_STORAGE_BUFFER
  } BufferBindingTargetBit;
  using BufferBindingTarget = uint32_t;

  using DeviceMemoryFence = void*;
  typedef enum
  {
    FENCE_ALREADY_SIGNALED = 0x911A,
    FENCE_TIMEOUT_EXPIRED = 0x911B,
    FENCE_CONDITION_SATISFIED = 0x911C,
    FENCE_WAIT_FAILED = 0x911D
  } FenceSignalBits;
  using FenceSignal = uint32_t;

  struct Buffer
  {
    RendererID RendererID;
    BufferDescriptor Descriptor;
    void* MemoryPtr;

    void Lock();
    void Wait();

    Buffer(Device& device, Pistachio::RendererID rendererId, BufferDescriptor descriptor, void* memoryPtr)
      : m_Device(device), RendererID(rendererId), Descriptor(descriptor), MemoryPtr(memoryPtr)
    {
      PSTC_INFO("GPU mapped buffer ({0}) created!", RendererID);
    }

    ~Buffer();

  private:
    Buffer() = delete;
    Device& m_Device;
    
    DeviceMemoryFence gSync = nullptr;
  };

  /////////////////////////////////////////////////////////
  /// Attribute Layout / Vertex Array Object (VAO)
  /////////////////////////////////////////////////////////

  struct AttributeLayoutDescriptorEntry
  {
    Ref<Buffer> Buffer;
    uint32_t BindingFirst;
    uint32_t BindingCount;
    BufferMemoryLayout MemoryLayout;
  };
  using AttributeLayoutDescriptor = std::vector<AttributeLayoutDescriptorEntry>;

  struct AttributeLayout
  {
    RendererID RendererID;
    AttributeLayoutDescriptor Descriptor;
  };


  /////////////////////////////////////////////////////////
  /// Textures
  /////////////////////////////////////////////////////////

  typedef enum 
  {
    // Default values from OpenGL
    FILTER_NEAREST = 0x2600,
    FILTER_LINEAR = 0x2601,
    FILTER_NEAREST_MIPMAP_NEAREST = 0x2700,
    FILTER_LINEAR_MIPMAP_NEAREST = 0x2701,
    FILTER_NEAREST_MIPMAP_LINEAR = 0x2702,
    FILTER_LINEAR_MIPMAP_LINEAR = 0x2703
  } TextureFilterBits;
  using TextureFilter = uint32_t;

  typedef enum 
  {
    INTERNAL_FORMAT_R8 = 0x8229,
    INTERNAL_FORMAT_R16 = 0x822A,
    INTERNAL_FORMAT_RG8 = 0x822B,
    INTERNAL_FORMAT_RG16 = 0x822C,
    INTERNAL_FORMAT_R16F = 0x822D,
    INTERNAL_FORMAT_R32F = 0x822E,
    INTERNAL_FORMAT_RG16F = 0x822F,
    INTERNAL_FORMAT_RG32F = 0x8230,
    INTERNAL_FORMAT_R8I = 0x8231,
    INTERNAL_FORMAT_R8UI = 0x8232,
    INTERNAL_FORMAT_R16I = 0x8233,
    INTERNAL_FORMAT_R16UI = 0x8234,
    INTERNAL_FORMAT_R32I = 0x8235,
    INTERNAL_FORMAT_R32UI = 0x8236,
    INTERNAL_FORMAT_RG8I = 0x8237,
    INTERNAL_FORMAT_RG8UI = 0x8238,
    INTERNAL_FORMAT_RG16I = 0x8239,
    INTERNAL_FORMAT_RG16UI = 0x823A,
    INTERNAL_FORMAT_RG32I = 0x823B,
    INTERNAL_FORMAT_RG32UI = 0x823C,
    INTERNAL_FORMAT_R3_G3_B2 = 0x2A10,
    INTERNAL_FORMAT_RGB4 = 0x804F,
    INTERNAL_FORMAT_RGB5 = 0x8050,
    INTERNAL_FORMAT_RGB8 = 0x8051,
    INTERNAL_FORMAT_RGB10 = 0x8052,
    INTERNAL_FORMAT_RGB12 = 0x8053,
    INTERNAL_FORMAT_RGB16 = 0x8054,
    INTERNAL_FORMAT_RGBA2 = 0x8055,
    INTERNAL_FORMAT_RGBA4 = 0x8056,
    INTERNAL_FORMAT_RGB5_A1 = 0x8057,
    INTERNAL_FORMAT_RGBA8 = 0x8058,
    INTERNAL_FORMAT_RGB10_A2 = 0x8059,
    INTERNAL_FORMAT_RGBA12 = 0x805A,
    INTERNAL_FORMAT_RGBA16 = 0x805B,
    INTERNAL_FORMAT_UNSIGNED_BYTE_3_3_2 = 0x8032,
    INTERNAL_FORMAT_UNSIGNED_SHORT_4_4_4_4 = 0x8033,
    INTERNAL_FORMAT_UNSIGNED_SHORT_5_5_5_1 = 0x8034,
    INTERNAL_FORMAT_UNSIGNED_INT_8_8_8_8 = 0x8035,
    INTERNAL_FORMAT_UNSIGNED_INT_10_10_10_2 = 0x8036,
    INTERNAL_FORMAT_DEPTH_COMPONENT_16 = 0x81A5,
    INTERNAL_FORMAT_DEPTH_COMPONENT_24 = 0x81A6,
    INTERNAL_FORMAT_DEPTH_COMPONENT_32 = 0x81A7,
    INTERNAL_FORMAT_DEPTH_COMPONENT_32F = 0x8CAC
  } TextureInternalFormatBits;
  using TextureInternalFormat = uint32_t;

  typedef enum
  {
    TEXTURE_1D = 0x0DE0,
    TEXTURE_2D = 0x0DE1,
    TEXTURE_3D = 0x806F,
    TEXTURE_BUFFER = 0x8C2A,
    TEXTURE_1D_ARRAY = 0x8C18,
    TEXTURE_2D_ARRAY = 0x8C1A,
    TEXTURE_2D_MULTISAMPLE = 0x9100,
    TEXTURE_2D_MULTISAMPLE_ARRAY = 0x9102
  } TextureTypeBits;
  using TextureTypeFlag = uint32_t;

  struct TextureDescriptorBase
  {
    uint32_t SizeX = 1, SizeY = 1, SizeZ = 0;
    uint32_t Samples = 1;
    uint32_t Layers = 1;
    TextureInternalFormat InternalFormat = INTERNAL_FORMAT_RGB8;
    TextureTypeFlag Type = TEXTURE_2D;
  };

  typedef enum
  {
    COLOR_ATTACHMENT_0 = 0x8CE0,
    COLOR_ATTACHMENT_1 = 0x8CE1,
    COLOR_ATTACHMENT_2 = 0x8CE2,
    COLOR_ATTACHMENT_3 = 0x8CE3,
    COLOR_ATTACHMENT_4 = 0x8CE4,
    COLOR_ATTACHMENT_5 = 0x8CE5,
    COLOR_ATTACHMENT_6 = 0x8CE6,
    COLOR_ATTACHMENT_7 = 0x8CE7,
    COLOR_ATTACHMENT_8 = 0x8CE8,
    COLOR_ATTACHMENT_9 = 0x8CE9,
    COLOR_ATTACHMENT_10 = 0x8CEA,
    COLOR_ATTACHMENT_11 = 0x8CEB,
    COLOR_ATTACHMENT_12 = 0x8CEC,
    COLOR_ATTACHMENT_13 = 0x8CED,
    COLOR_ATTACHMENT_14 = 0x8CEE,
    COLOR_ATTACHMENT_15 = 0x8CEF,
    COLOR_ATTACHMENT_16 = 0x8CF0,
    COLOR_ATTACHMENT_17 = 0x8CF1,
    COLOR_ATTACHMENT_18 = 0x8CF2,
    COLOR_ATTACHMENT_19 = 0x8CF3,
    COLOR_ATTACHMENT_20 = 0x8CF4,
    COLOR_ATTACHMENT_21 = 0x8CF5,
    COLOR_ATTACHMENT_22 = 0x8CF6,
    COLOR_ATTACHMENT_23 = 0x8CF7,
    COLOR_ATTACHMENT_24 = 0x8CF8,
    COLOR_ATTACHMENT_25 = 0x8CF9,
    COLOR_ATTACHMENT_26 = 0x8CFA,
    COLOR_ATTACHMENT_27 = 0x8CFB,
    COLOR_ATTACHMENT_28 = 0x8CFC,
    COLOR_ATTACHMENT_29 = 0x8CFD,
    COLOR_ATTACHMENT_30 = 0x8CFE,
    COLOR_ATTACHMENT_31 = 0x8CFF,
    DEPTH_ATTACHMENT = 0x8D00,
    STENCIL_ATTACHMENT = 0x8D20,
    DEPTH_STENCIL_ATTACHMENT = 0x821A
  } AttachmentPointBits;
  using AttachmentPoint = uint32_t;

  struct AttachmentDescriptor
  {
    AttachmentPoint AttachmentPoint = UINT32_MAX;
    TextureDescriptorBase TextureDescriptor;
  };

  struct Attachment
  {
    RendererID RendererID;
    AttachmentDescriptor AttachmentDescriptor;

    Attachment(Device& device, Pistachio::RendererID rendererId, Pistachio::AttachmentDescriptor attachmentDescriptor)
      : m_Device(device), RendererID(rendererId), AttachmentDescriptor(attachmentDescriptor)
    { }

    ~Attachment();

  private:
    friend class Sampler;
    Device& m_Device;
  };


  typedef enum
  {
    DEPTH_BUFFER = 0x00000100,
    STENCIL_BUFFER = 0x00000400,
    COLOR_BUFFER = 0x00004000
  } FramebufferMaskBits;
  using FramebufferMask = uint32_t;

  struct SamplerDescriptor
  {
    TextureDescriptorBase TextureDescriptor;
    uint32_t MipLevels = 1;
    TextureFilter MinFilter = FILTER_LINEAR;
    TextureFilter MagFilter = FILTER_LINEAR;
    // Todo: add wrap behaviour
  };

  struct Sampler
  {
    RendererID RendererID;
    SamplerDescriptor SamplerDescriptor;

    Sampler(Device& device, Pistachio::RendererID rendererId, Pistachio::SamplerDescriptor samplerDescriptor)
      : m_Device(device), RendererID(rendererId), SamplerDescriptor(samplerDescriptor)
    {}

    Sampler(Ref<Attachment> attachment, Pistachio::SamplerDescriptor samplerDescriptor)
      : m_Device(attachment->m_Device), RendererID(attachment->RendererID), SamplerDescriptor(samplerDescriptor)
    {}

    ~Sampler();

  private:
    Device& m_Device;
    Ref<Attachment> BaseAttachment; // Keep optional base attachment alive by keeping a reference to it
  };


  /////////////////////////////////////////////////////////
  /// Drawing
  /////////////////////////////////////////////////////////

  typedef enum
  {
    PRIMITIVE_TRIANGLES = 0x0004,
    PRIMITIVE_TRIANGLE_STRIP = 0x0005,
    PRIMITIVE_TRIANGLE_FAN = 0x0006,
    PRIMITIVE_LINES = 0x0001,
    PRIMITIVE_LINE_STRIP = 0x0003,
    PRIMITIVE_LINE_LOOP = 0x0002,
    PRIMITIVE_POINTS = 0x0000
  } DrawingPrimitiveBits;
  using DrawingPrimitive = uint32_t;



  /////////////////////////////////////////////////////////
  /// RenderGraph Resources
  /////////////////////////////////////////////////////////

  struct ShaderResource
  {
    std::string Name;
    std::vector<std::string> UsedInPasses;
  };

  struct RenderResourceBase
  {
    std::vector<std::string> ReadInPasses;
    std::vector<std::string> WrittenInPasses;
  };

  struct BufferResource : public RenderResourceBase {
    Ref<Buffer> Buffer;
  };

  struct AttachmentResource : public RenderResourceBase
  {
    Ref<Attachment> Attachment;
    SamplerDescriptor SamplerDescriptor;
  };

  struct SamplerResource : public RenderResourceBase
  {
    Ref<Sampler> Sampler;
  };
}



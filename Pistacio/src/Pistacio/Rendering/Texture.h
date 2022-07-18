#pragma once
#include "pch.h"

namespace Pistacio
{

  enum class TextureFilter
  {
    // Default values from OpenGL
    Nearest                 = 0x2600,
    Linear                  = 0x2601,
    Nearest_Mipmap_Nearest  = 0x2700,
    Linear_Mipmap_Nearest   = 0x2701,
    Nearest_Mipmap_Linear   = 0x2702,
    Linear_Mipmap_Linear    = 0x2703
  };

  enum class TextureGPUInternalFormat
  {
    // Default values from OpenGL
    RGB8  = 0x8051,
    RGBA8 = 0x8058,
    DepthComponent16 = 0x81A5,
    DepthComponent24 = 0x81A6,
    DepthComponent32 = 0x81A7,
    DepthComponent32F = 0x8CAC
  };

  struct TextureDescriptor
  {
    TextureGPUInternalFormat InternalFormat = TextureGPUInternalFormat::RGB8;
    TextureFilter MinFilter = TextureFilter::Linear_Mipmap_Nearest;
    TextureFilter MagFilter = TextureFilter::Linear_Mipmap_Nearest;
    uint32_t Width = -1;
    uint32_t Height = -1;
  };

  enum class ImageFormat
  {
    // Default values from OpenGL
    RGB   = 0x1907,
    RGBA  = 0x1908
  };

  enum class ImageDataType
  {
    // Default values from OpenGL
    Byte              = 0x1400,
    UnsignedByte     = 0x1401,
    Short             = 0x1402,
    UnsignedShort    = 0x1403,
    Int               = 0x1404,
    UnsignedInt       = 0x1405,
    Float             = 0x1406
  };

  struct Image
  {
    ImageFormat Format = ImageFormat::RGB;
    ImageDataType DataType = ImageDataType::UnsignedByte;
    uint32_t Width = -1;
    uint32_t Height = -1;
    void* Data = nullptr;
  };

  class Texture2D
  {
  public:
    virtual ~Texture2D() { }

    virtual TextureDescriptor GetDescriptor() const = 0;
    virtual void SetData(Image image) const = 0;
    virtual const RendererID GetRendererID() const = 0;

    static Ref<Texture2D> Create(const std::string& path, TextureDescriptor descriptor);
    static Ref<Texture2D> Create(TextureDescriptor descriptor);
  };

}


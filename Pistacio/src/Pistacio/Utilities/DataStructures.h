#pragma once
#include <vector>
#include "Memory.h"

namespace Pistacio
{
  template<typename T>
  struct ReadOnlyVector
  {
  public:
    ReadOnlyVector(std::initializer_list<T> m_Entries) : m_Entries(m_Entries)
    {
    }

    ReadOnlyVector(std::vector<T>& m_Entries) : m_Entries(m_Entries)
    {
    }

    ReadOnlyVector(ReadOnlyVector<T>& m_Entries) : m_Entries(m_Entries)
    {
    }

    std::vector<T>::iterator begin() { return m_Entries.begin(); }
    std::vector<T>::iterator end() { return m_Entries.end(); }
    std::vector<T>::reverse_iterator rbegin() { return m_Entries.rbegin(); }
    std::vector<T>::reverse_iterator rend() { return m_Entries.rend(); }
    std::vector<T>::const_iterator begin() const { return m_Entries.begin(); }
    std::vector<T>::const_iterator end() const { return m_Entries.end(); }
    std::vector<T>::const_reverse_iterator rbegin() const { return m_Entries.rbegin(); }
    std::vector<T>::const_reverse_iterator rend() const { return m_Entries.rend(); }

  private:
    std::vector<T> m_Entries;

  };


  typedef enum 
  {
    // Default values from OpenGL
    IMAGE_FORMAT_RGB = 0x1907,
    IMAGE_FORMAT_RGBA = 0x1908
  } ImageFormatBits;
  using ImageFormat = uint32_t;

  typedef enum
  {
    // Default values from OpenGL
    Byte = 0x1400,
    UnsignedByte = 0x1401,
    Short = 0x1402,
    UnsignedShort = 0x1403,
    Int = 0x1404,
    UnsignedInt = 0x1405,
    Float = 0x1406
  } PrimitiveDataTypeBits;
  using PrimitiveDataType = uint32_t;

  struct Image // 160 bit
  {
    ImageFormat Format = IMAGE_FORMAT_RGB;
    PrimitiveDataType DataType = UnsignedByte;
    uint32_t Width = 0;
    uint32_t Height = 0;
    void* Data = nullptr;

    //~Image();
  };



  


}


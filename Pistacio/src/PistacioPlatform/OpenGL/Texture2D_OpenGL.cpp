
#include "pch.h"
#include "Texture2D_OpenGL.h"
#include "stb_image.h"

namespace Pistacio
{
  Texture2D_OpenGL::Texture2D_OpenGL(const std::string& path, TextureDescriptor descriptor) : Descriptor(descriptor)
  {
   
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    PSTC_CORE_ASSERT(data, "Failed to load image!");
    Descriptor.Width = width;
    Descriptor.Height = height;

    Image image;
    image.Data = data;
    image.Width = width;
    image.Height = height;
    image.DataType = ImageDataType::UnsignedByte;

    if (channels == 3)
    {
      descriptor.InternalFormat = TextureGPUInternalFormat::RGB8;
      image.Format = ImageFormat::RGB;
    }
    else if (channels == 4)
    {
      descriptor.InternalFormat = TextureGPUInternalFormat::RGBA8;
      image.Format = ImageFormat::RGBA;
    }
    else
    {
      PSTC_CORE_ASSERT(false, "Unrecognised image format!");
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
    glTextureStorage2D(m_RendererId, 1, static_cast<GLenum>(descriptor.InternalFormat), width, height);

    glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(descriptor.MinFilter));
    glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(descriptor.MagFilter));

    SetData(image);

    stbi_image_free(data);
  }

  Texture2D_OpenGL::Texture2D_OpenGL(TextureDescriptor descriptor) : Descriptor(descriptor)
  {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
    glTextureStorage2D(m_RendererId, 1, static_cast<GLenum>(descriptor.InternalFormat), descriptor.Width, descriptor.Height);

    glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(descriptor.MagFilter));
    glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(descriptor.MinFilter));
  }

  //void Texture2D_OpenGL::Bind(uint32_t unit) const
  //{
  //  glBindTextureUnit(unit, m_RendererId);
  //}

  const RendererID Texture2D_OpenGL::GetRendererID() const
  {
    return m_RendererId;
  }

  void Texture2D_OpenGL::SetData(Image image) const
  {
    glTextureSubImage2D(
      m_RendererId,
      0, 0, 0,
      image.Width, image.Height,
      static_cast<GLenum>(image.Format),
      static_cast<GLenum>(image.DataType),
      image.Data
    );
  }

}

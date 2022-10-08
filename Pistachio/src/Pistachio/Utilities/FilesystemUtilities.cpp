#include "pch.h"
#include "FilesystemUtilities.h"
#include "stb_image.h"

namespace Pistachio
{

	Image LoadImageFromFile(const std::string& path)
	{

		Image image;

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		PSTC_CORE_ASSERT(data, "Failed to load image!");

		image.Data = data;
		image.Width = width;
		image.Height = height;
		image.DataType = UnsignedByte;

		if (channels == 3)
		{
			image.Format = IMAGE_FORMAT_RGB;
		}
		else if (channels == 4)
		{
			image.Format = IMAGE_FORMAT_RGBA;
		}
		else
		{
			PSTC_CORE_ASSERT(false, "Unrecognised image format!");
		}

		return image;

	}

}

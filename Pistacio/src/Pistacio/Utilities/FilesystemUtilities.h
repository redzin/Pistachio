#pragma once
#include "pch.h"


namespace Pistacio
{
	Image LoadImageFromFile(const std::string& path);
}

template<class T>
inline T base_name(T const& path, T const& delims = "/\\")
{
	return path.substr(path.find_last_of(delims) + 1);
}

template<class T>
inline T remove_extension(T const& filename)
{
	typename T::size_type const p(filename.find_last_of('.'));
	return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
}

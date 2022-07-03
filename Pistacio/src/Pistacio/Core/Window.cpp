#include "pch.h"
#include "Window.h"

#ifdef PSTC_PLATFORM_WINDOWS
#include "Pistacio/Platform/Windows/WindowsWindow.h"

namespace Pistacio
{
	std::unique_ptr<Window> Window::Create(const WindowProperties& props)
	{
#ifdef PSTC_PLATFORM_WINDOWS
		return std::unique_ptr<Window>(new WindowsWindow(props));
#else
		PSTC_CORE_FATAL("Unknown
		throw std::exception("Unsupported platform!"); platform!");		
#endif
}
}
#endif
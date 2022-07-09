#include "pch.h"
#include "Window.h"

#ifdef PSTC_PLATFORM_WINDOWS
#include "PistacioPlatform/Windows/WindowsWindow.h"

namespace Pistacio
{
	std::unique_ptr<Window> Window::Create(std::string windowName, uint32_t width, uint32_t height, bool hintFloat)
	{
#ifdef PSTC_PLATFORM_WINDOWS
		return std::unique_ptr<Window>(new WindowsWindow(windowName, width, height, hintFloat));
#else
		PSTC_CORE_FATAL("Unknown
		throw std::exception("Unsupported platform!"); platform!");		
#endif
}
}
#endif
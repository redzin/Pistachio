#pragma once

#include <stdio.h>
#include <memory>
#include <algorithm>
#include <map>
#include <functional>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <filesystem>


#include "Pistacio/Core/Core.h"
#include "Pistacio/Logging/Log.h"
#include "Pistacio/Core/Input.h"
#include "Pistacio/Events/Event.h"
#include "Pistacio/Core/Window.h"
#include "Pistacio/Core/LayerStack.h"
#include "Pistacio/Platform/OpenGL/ImGuiRenderer.h"


#ifdef PSTC_PLATFORM_WINDOWS
  #include <Windows.h>
#endif






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
#include <optional>
#include <typeindex>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Pistacio/Core/Core.h"
#include "Pistacio/Core/Log.h"
#include "Pistacio/Core/Input.h"
#include "Pistacio/Core/Events.h"
#include "Pistacio/Core/Window.h"
#include "Pistacio/Core/Layers/LayerStack.h"


#ifdef PSTC_PLATFORM_WINDOWS
#include "PistacioPlatform/OpenGL/ImGuiRenderer_GLFW_OpenGL.h"
#include <Windows.h>
#endif






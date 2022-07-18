#pragma once

// Meta
#include <typeindex>

// IO
#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <stdexcept>

// Memory
#include <memory>

// Algorithms & Data Structures
#include <algorithm>
#include <map>
#include <functional>
#include <chrono>
#include <optional>
#include <tuple>
#include <array>

//Concurrency
#include <thread>

// Logging
#include <spdlog/spdlog.h>

// Math
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Pistacio
using RendererID = unsigned int;
#include "Pistacio/Utilities/FilesystemUtilities.h"
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






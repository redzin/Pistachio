#pragma once

// Meta
#include <typeindex>

// IO
#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <fstream>

// Memory
#include <memory>

// Time
#include <chrono>

// Algorithms & Data Structures
#include <algorithm>
#include <map>
#include <unordered_map>
#include <functional>
#include <optional>
#include <tuple>
#include <array>
#include <set>
#include <unordered_set>

//Concurrency
#include <thread>

// Logging
#include <spdlog/spdlog.h>

// Math
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Pistachio
#include "Pistachio/Core/Core.h"
#include <Pistachio/Utilities/Timer.h>
#include "Pistachio/Utilities/Hasher.h"
#include "Pistachio/Utilities/DataStructures.h"
#include "Pistachio/Core/Log.h"
#include "Pistachio/Utilities/FilesystemUtilities.h"
#include "Pistachio/Core/Input.h"
#include "Pistachio/Core/Events.h"
#include "Pistachio/Core/Window.h"
#include "Pistachio/Core/Layers/Layer.h"
#include "Pistachio/Core/Layers/LayerStack.h"
#include "Pistachio/Rendering/RenderGraph.h"


#ifdef PSTC_PLATFORM_WINDOWS
#include "PistachioPlatform/OpenGL/ImGuiRenderer_GLFW_OpenGL.h"
#include <Windows.h>
#endif






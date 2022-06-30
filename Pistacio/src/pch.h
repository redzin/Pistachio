#pragma once

#include <memory>
#include <algorithm>
#include <map>
#include <functional>
#include <spdlog/spdlog.h>
#include <stdexcept>


#include "Pistacio/Input.h"
#include "Pistacio/Logging/Log.h"
#include "Pistacio/Events/Event.h"

#ifdef PSTC_PLATFORM_WINDOWS
  #include <Windows.h>
#endif


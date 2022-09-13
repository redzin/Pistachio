#pragma once

#include "pch.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#ifdef PSTC_DEBUG

namespace Pistacio::Logger {

  void Init();
  Ref<spdlog::logger> GetCoreLogger();
  Ref<spdlog::logger> GetClientLogger();
}

#define INIT_LOGGER() Logger::Init();

#define PSTC_TRACE(...) Pistacio::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define PSTC_INFO(...) Pistacio::Logger::GetClientLogger()->info(__VA_ARGS__)
#define PSTC_WARN(...) Pistacio::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define PSTC_ERROR(...) Pistacio::Logger::GetClientLogger()->error(__VA_ARGS__)
#define PSTC_FATAL(...) Pistacio::Logger::GetClientLogger()->critical(__VA_ARGS__)

#define PSTC_CORE_TRACE(...) Pistacio::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define PSTC_CORE_INFO(...) Pistacio::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define PSTC_CORE_WARN(...) Pistacio::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define PSTC_CORE_ERROR(...) Pistacio::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define PSTC_CORE_FATAL(...) Pistacio::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#else

#define INIT_LOGGER

#define PSTC_TRACE(...)
#define PSTC_INFO(...)
#define PSTC_WARN(...)
#define PSTC_ERROR(...)
#define PSTC_FATAL(...)

#define PSTC_CORE_TRACE(...)
#define PSTC_CORE_INFO(...)
#define PSTC_CORE_WARN(...)
#define PSTC_CORE_ERROR(...)
#define PSTC_CORE_FATAL(...)

#endif
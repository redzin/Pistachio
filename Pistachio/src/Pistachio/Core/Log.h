#pragma once

#include "pch.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#ifdef PSTC_DEBUG

namespace Pistachio::Logger {

  void Init();
  Ref<spdlog::logger> GetCoreLogger();
  Ref<spdlog::logger> GetClientLogger();
}

#define INIT_LOGGER() Logger::Init();

#define PSTC_TRACE(...) Pistachio::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define PSTC_INFO(...) Pistachio::Logger::GetClientLogger()->info(__VA_ARGS__)
#define PSTC_WARN(...) Pistachio::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define PSTC_ERROR(...) Pistachio::Logger::GetClientLogger()->error(__VA_ARGS__)
#define PSTC_FATAL(...) Pistachio::Logger::GetClientLogger()->critical(__VA_ARGS__)

#define PSTC_CORE_TRACE(...) Pistachio::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define PSTC_CORE_INFO(...) Pistachio::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define PSTC_CORE_WARN(...) Pistachio::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define PSTC_CORE_ERROR(...) Pistachio::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define PSTC_CORE_FATAL(...) Pistachio::Logger::GetCoreLogger()->critical(__VA_ARGS__)

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
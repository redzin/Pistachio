#pragma once

#include <memory>
#include "Core.h"
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Pistacio::Logger {
  std::shared_ptr<spdlog::logger> s_CoreLogger;
  std::shared_ptr<spdlog::logger> s_ClientLogger;

  void PSTC_API Init();
  std::shared_ptr<spdlog::logger> PSTC_API GetCoreLogger();
  std::shared_ptr<spdlog::logger> PSTC_API GetClientLogger();
}

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


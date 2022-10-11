#include "pch.h"
#include "Log.h"

#ifdef PSTC_DEBUG

namespace Pistachio::Logger {

  std::shared_ptr<spdlog::logger> s_CoreLogger;
  std::shared_ptr<spdlog::logger> s_ClientLogger;

  void Pistachio::Logger::Init()
  {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_CoreLogger = spdlog::stdout_color_mt("Pistachio");
    s_CoreLogger->set_level(spdlog::level::trace);

    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);
  }

  std::shared_ptr<spdlog::logger> GetCoreLogger()
  {
    return s_CoreLogger;
  }

  std::shared_ptr<spdlog::logger> GetClientLogger()
  {
    return s_ClientLogger;
  }
}

#endif


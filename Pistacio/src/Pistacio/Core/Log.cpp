#include "pch.h"
#include "Log.h"

#ifdef PSTC_DEBUG

namespace Pistacio::Logger {

  Ref<spdlog::logger> s_CoreLogger;
  Ref<spdlog::logger> s_ClientLogger;

  void Pistacio::Logger::Init()
  {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_CoreLogger = spdlog::stdout_color_mt("Pistacio");
    s_CoreLogger->set_level(spdlog::level::trace);

    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);
  }

  Ref<spdlog::logger> GetCoreLogger()
  {
    return s_CoreLogger;
  }

  Ref<spdlog::logger> GetClientLogger()
  {
    return s_ClientLogger;
  }
}

#endif


#include "pch.h"
#include "Log.h"

#ifdef PSTC_DEBUG

namespace Pistacio::Logger {

  std::shared_ptr<spdlog::logger> s_CoreLogger;
  std::shared_ptr<spdlog::logger> s_ClientLogger;

}

void Pistacio::Logger::Init()
{
  spdlog::set_pattern("%^[%T] %n: %v%$");
  s_CoreLogger = spdlog::stdout_color_mt("Pistacio");
  s_CoreLogger->set_level(spdlog::level::trace);

  s_ClientLogger = spdlog::stdout_color_mt("APP");
  s_ClientLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger> Pistacio::Logger::GetCoreLogger()
{
  return s_CoreLogger;
}

std::shared_ptr<spdlog::logger> Pistacio::Logger::GetClientLogger()
{
  return s_ClientLogger;
}

#endif


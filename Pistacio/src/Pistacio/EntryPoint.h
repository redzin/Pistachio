#pragma once
#include "Application.h"
#include "Log.h"

#ifdef PSTC_PLATFORM_WINDOWS

extern Pistacio::Application* Pistacio::CreateApplication();


int main(int argc, char** argv)
{
  Pistacio::Logger::Init();
  PSTC_CORE_INFO("Logger initialized!");

  Pistacio::Application* app = Pistacio::CreateApplication();
  app->Run();
  delete app;
}

#endif
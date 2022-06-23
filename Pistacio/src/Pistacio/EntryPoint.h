#pragma once
#include "Application.h"

#ifdef PSTC_PLATFORM_WINDOWS

extern Pistacio::Application* Pistacio::CreateApplication();

int main(int argc, char** argv)
{
  Pistacio::Application* app = Pistacio::CreateApplication();
  app->Run();
  delete app;
}

#endif

#pragma once
#include "pch.h"
#include "Application.h"

#ifdef PSTC_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
  std::unique_ptr<Pistacio::Application> app = Pistacio::ApplicationFactory::Create();
  app->Run();
}

#endif

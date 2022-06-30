#pragma once

#ifdef PSTC_PLATFORM_WINDOWS
  #ifdef PSTC_BUILD_DLL
    #define PSTC_API __declspec(dllexport)
  #else
    #define PSTC_API __declspec(dllimport)
  #endif
#else
  #error Pistacio only supports Windows!
#endif

#define BIT(x) (1 << x - 1)


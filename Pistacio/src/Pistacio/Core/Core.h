#pragma once

#ifdef PSTC_PLATFORM_WINDOWS
  #ifdef PSTC_BUILD_DLL
    //#define PSTC_API __declspec(dllexport)
    #define PSTC_API
  #else
    #define PSTC_API
    //#define PSTC_API __declspec(dllimport)
  #endif
#else
  #error Pistacio only supports Windows!
#endif

#define BIT(x) (1 << x - 1)

#ifdef PSTC_DEBUG

  #define PSTC_DEBUGBREAK() __debugbreak();
  #define PSTC_ASSERT(x, ...) if (!x) { PSTC_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); }
  #define PSTC_CORE_ASSERT(x, ...) if (!x) { PSTC_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); }

#else

  #define PSTC_DEBUGBREAK() 
  #define PSTC_ASSERT(x, ...) 
  #define PSTC_CORE_ASSERT(x, ...) 

#endif


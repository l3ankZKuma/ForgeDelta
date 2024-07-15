#pragma once

#include<memory>               
#include<functional>


#define FD_CORE_ASSERT(x, ...) { if(!(x)) { FD_CORE_FATAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define FD_ASSERT(x, ...) { if(!(x)) { FD_FATAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define BIT(x) (1u << x)

#ifdef FD_PLATFORM_WINDOWS
  #ifdef FD_BUILD_DLL
    #define FORGEDELTA_API __declspec(dllexport)
  #else
    #define FORGEDELTA_API __declspec(dllimport)
  #endif
#endif




namespace ForgeDelta {

  template<typename T>
  using Scope = std::unique_ptr<T>;

  template<typename T>
  using Ref = std::shared_ptr<T>;


}


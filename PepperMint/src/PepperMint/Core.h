#pragma once

#ifdef PM_PLATFORM_WINDOWS
	#ifdef PM_DYNAMIC_LINK
		#ifdef PM_BUILD_DLL
			#define PM_API __declspec(dllexport)
		#else
			#define PM_API __declspec(dllimport)
		#endif // PM_BUILD_DLL
	#else
		#define PM_API
	#endif // PM_DYNAMIC_LINK
#else
	#error PepperMint only support Windows!
#endif // PM_PLATFORM_WINDOWS

#ifdef PM_DEBUG
	#define PM_ENABLE_ASSERTS
#endif // PM_DEBUG


#ifdef PM_ENABLE_ASSERTS
	#define PM_ASSERT(x, ...) { if(!(x)) { PM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define PM_CORE_ASSERT(x, ...) { if(!(x)) { PM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define PM_ASSERT(x, ...)
	#define PM_CORE_ASSERT(x, ...)
#endif // PM_ENABLE_ASSERTS

#define PM_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (1 << x)
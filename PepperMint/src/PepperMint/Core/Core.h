#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define PM_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* 
	 * TARGET_OS_MAC exists on all the platforms so we must check all of them (in this order) 
	 * to ensure that we're running on MAC and not some other Apple platform 
	 */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define PM_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define PM_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
	 /* 
	  * We also have to check __ANDROID__ before __linux__ since android is based on the
	  * linux kernel it has __linux__ defined 
	  */
#elif defined(__ANDROID__)
	#define PM_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define PM_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection

#ifndef PM_PLATFORM_WINDOWS
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

namespace PepperMint {

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;
}
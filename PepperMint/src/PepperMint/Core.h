#pragma once

#ifdef PM_PLATFORM_WINDOWS
	#ifdef PM_BUILD_DLL
		#define PM_API __declspec(dllexport)
	#else
		#define PM_API __declspec(dllimport)
	#endif // PM_BUILD_DLL
#else
	#error PepperMint only support Windows!
#endif // PM_PLATFORM_WINDOWS

#define BIT(x) (1 << x)
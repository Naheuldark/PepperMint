#include "pmpch.h"
#include "PepperMint/Core/Input.h"

#ifdef PM_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif // PM_PLATFORM_WINDOWS

namespace PepperMint {

Scope<Input> Input::sInstance = Input::Create();

Scope<Input> Input::Create() {
	#ifdef PM_PLATFORM_WINDOWS
	return CreateScope<WindowsInput>();
	#else
	PM_CORE_ASSERT(false, "Unknown platform!");
	return nullptr;
	#endif
}
}
#include "pmpch.h"
#include "PepperMint/Core/Window.h"

#ifdef PM_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif // PM_PLATFORM_WINDOWS

namespace PepperMint {

Scope<Window> Window::Create(const WindowProperties& iProperties) {
	#ifdef PM_PLATFORM_WINDOWS
	return CreateScope<WindowsWindow>(iProperties);
	#else
	PM_CORE_ASSERT(false, "Unknown platform!");
	return nullptr;
	#endif
}
}
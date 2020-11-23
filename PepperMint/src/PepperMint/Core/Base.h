#pragma once

#include <memory>

#include "PepperMint/Core/PlatformDetection.h"

#ifdef PM_DEBUG
#if defined(PM_PLATFORM_WINDOWS)
#define PM_DEBUGBREAK() __debugbreak()
#elif defined(PM_PLATFORM_LINUX)
#include <signal.h>
#define PM_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform does not support debugbreak yet!"
#endif
#define PM_ENABLE_ASSERTS
#else
#define PM_DEBUGBREAK()
#endif // End of debugbreak and debug mode

#define PM_EXPAND_MACRO(x) x
#define PM_STRINGIFY_MACRO(x) #x

#define PM_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define BIT(x) (1 << x)

namespace PepperMint {

// Unique / Scope pointer
template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// Shared / Reference pointer
template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
}

#include "PepperMint/Core/Assert.h"
#include "PepperMint/Core/Log.h"

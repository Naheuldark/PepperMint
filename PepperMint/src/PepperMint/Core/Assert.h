#pragma once

#include "PepperMint/Core/Base.h"
#include "PepperMint/Core/Log.h"

#ifdef PM_ENABLE_ASSERTS
namespace PepperMint::Assert {
// Returns the simple filename rather than the full path
constexpr const char* currentFilename(const char* iPath) {
    const char* file = iPath;
    while (*iPath) {
        if (*iPath == '/' || *iPath == '\\')
            file = ++iPath;
        else
            iPath++;
    }
    return file;
}
}

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and provide support for custom formatting by concatenating
// the formatting string instead of having the format inside the default message
#define PM_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
    {                                                  \
        if (!(check)) {                                \
            PM##type##ERROR(msg, __VA_ARGS__);         \
            PM_DEBUGBREAK();                           \
        }                                              \
    }
#define PM_INTERNAL_ASSERT_WITH_MSG(type, check, ...) PM_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define PM_INTERNAL_ASSERT_NO_MSG(type, check) \
    PM_INTERNAL_ASSERT_IMPL(                   \
        type, check, "Assertion '{0}' failed at {1}:{2}", PM_STRINGIFY_MACRO(check), ::Hazel::Assert::CurrentFileName(__FILE__), __LINE__)

#define PM_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define PM_INTERNAL_ASSERT_GET_MACRO(...) \
    PM_EXPAND_MACRO(PM_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, PM_INTERNAL_ASSERT_WITH_MSG, PM_INTERNAL_ASSERT_NO_MSG))

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define PM_ASSERT(...) PM_EXPAND_MACRO(PM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define PM_CORE_ASSERT(...) PM_EXPAND_MACRO(PM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
#define PM_ASSERT(...)
#define PM_CORE_ASSERT(...)
#endif // PM_ENABLE_ASSERTS

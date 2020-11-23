#pragma once

#include <filesystem>

#include "PepperMint/Core/Base.h"
#include "PepperMint/Core/Log.h"

#ifdef PM_ENABLE_ASSERTS
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
        type, check, "Assertion '{0}' failed at {1}:{2}", PM_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define PM_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define PM_INTERNAL_ASSERT_GET_MACRO(...) \
    PM_EXPAND_MACRO(PM_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, PM_INTERNAL_ASSERT_WITH_MSG, PM_INTERNAL_ASSERT_NO_MSG))

#define PM_ASSERT(...) PM_EXPAND_MACRO(PM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define PM_CORE_ASSERT(...) PM_EXPAND_MACRO(PM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
#define PM_ASSERT(...)
#define PM_CORE_ASSERT(...)
#endif // PM_ENABLE_ASSERTS

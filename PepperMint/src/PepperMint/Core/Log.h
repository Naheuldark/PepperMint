#pragma once

#include "PepperMint/Core/Base.h"

#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace PepperMint {

class Log {
  public:
    Log()  = default;
    ~Log() = default;

    static void Init();

    static Ref<spdlog::logger>& CoreLogger() { return _coreLogger; }
    static Ref<spdlog::logger>& ClientLogger() { return _clientLogger; }

  private:
    static Ref<spdlog::logger> _coreLogger;
    static Ref<spdlog::logger> _clientLogger;
};
}

// Custom Log Functions
template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector) {
    return os << glm::to_string(vector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix) {
    return os << glm::to_string(matrix);
}

template <typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quat) {
    return os << glm::to_string(quat);
}

// Core Log Macros
#define PM_CORE_FATAL(...) ::PepperMint::Log::CoreLogger()->critical(__VA_ARGS__);
#define PM_CORE_ERROR(...) ::PepperMint::Log::CoreLogger()->error(__VA_ARGS__);
#define PM_CORE_WARN(...) ::PepperMint::Log::CoreLogger()->warn(__VA_ARGS__);
#define PM_CORE_INFO(...) ::PepperMint::Log::CoreLogger()->info(__VA_ARGS__);
#define PM_CORE_TRACE(...) ::PepperMint::Log::CoreLogger()->trace(__VA_ARGS__);

// Client Log Macros
#define PM_FATAL(...) ::PepperMint::Log::ClientLogger()->critical(__VA_ARGS__);
#define PM_ERROR(...) ::PepperMint::Log::ClientLogger()->error(__VA_ARGS__);
#define PM_WARN(...) ::PepperMint::Log::ClientLogger()->warn(__VA_ARGS__);
#define PM_INFO(...) ::PepperMint::Log::ClientLogger()->info(__VA_ARGS__);
#define PM_TRACE(...) ::PepperMint::Log::ClientLogger()->trace(__VA_ARGS__);

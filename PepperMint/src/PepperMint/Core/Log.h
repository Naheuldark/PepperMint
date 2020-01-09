#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace PepperMint {

class Log {
public:
	Log() = default;
	~Log() = default;

	static void Init();
	
	inline static std::shared_ptr<spdlog::logger>& CoreLogger() { return _coreLogger; }
	inline static std::shared_ptr<spdlog::logger>& ClientLogger() { return _clientLogger; }

private:
	static std::shared_ptr<spdlog::logger> _coreLogger;
	static std::shared_ptr<spdlog::logger> _clientLogger;
};
}

// Core Log Macros
#define PM_CORE_FATAL(...) ::PepperMint::Log::CoreLogger()->critical(__VA_ARGS__);
#define PM_CORE_ERROR(...) ::PepperMint::Log::CoreLogger()->error(__VA_ARGS__);
#define PM_CORE_WARN(...)  ::PepperMint::Log::CoreLogger()->warn(__VA_ARGS__);
#define PM_CORE_INFO(...)  ::PepperMint::Log::CoreLogger()->info(__VA_ARGS__);
#define PM_CORE_TRACE(...) ::PepperMint::Log::CoreLogger()->trace(__VA_ARGS__);

// Client Log Macros
#define PM_FATAL(...) ::PepperMint::Log::ClientLogger()->critical(__VA_ARGS__);
#define PM_ERROR(...) ::PepperMint::Log::ClientLogger()->error(__VA_ARGS__);
#define PM_WARN(...)  ::PepperMint::Log::ClientLogger()->warn(__VA_ARGS__);
#define PM_INFO(...)  ::PepperMint::Log::ClientLogger()->info(__VA_ARGS__);
#define PM_TRACE(...) ::PepperMint::Log::ClientLogger()->trace(__VA_ARGS__);
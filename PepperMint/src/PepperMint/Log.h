#pragma once

#include <memory>

#include "Core.h"

#include "spdlog/spdlog.h"

namespace PepperMint {

class PM_API Log {
public:
	Log() = default;
	~Log() = default;

	static void Init();
	
	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return _coreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _clientLogger; }

private:
	static std::shared_ptr<spdlog::logger> _coreLogger;
	static std::shared_ptr<spdlog::logger> _clientLogger;
};
}

// Core Log Macros
#define PM_CORE_FATAL(...) ::PepperMint::Log::GetCoreLogger()->critical(__VA_ARGS__);
#define PM_CORE_ERROR(...) ::PepperMint::Log::GetCoreLogger()->error(__VA_ARGS__);
#define PM_CORE_WARN(...)  ::PepperMint::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define PM_CORE_INFO(...)  ::PepperMint::Log::GetCoreLogger()->info(__VA_ARGS__);
#define PM_CORE_TRACE(...) ::PepperMint::Log::GetCoreLogger()->trace(__VA_ARGS__);

// Client Log Macros
#define PM_FATAL(...) ::PepperMint::Log::GetClientLogger()->critical(__VA_ARGS__);
#define PM_ERROR(...) ::PepperMint::Log::GetClientLogger()->error(__VA_ARGS__);
#define PM_WARN(...)  ::PepperMint::Log::GetClientLogger()->warn(__VA_ARGS__);
#define PM_INFO(...)  ::PepperMint::Log::GetClientLogger()->info(__VA_ARGS__);
#define PM_TRACE(...) ::PepperMint::Log::GetClientLogger()->trace(__VA_ARGS__);
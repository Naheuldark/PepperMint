#include "PepperMint/Core/Log.h"
#include "pmpch.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace PepperMint {

Ref<spdlog::logger> Log::_coreLogger;
Ref<spdlog::logger> Log::_clientLogger;

void Log::Init() {
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("PepperMint.log", true));

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");

    _coreLogger = std::make_shared<spdlog::logger>("PEPPERMINT", begin(logSinks), end(logSinks));
    spdlog::register_logger(_coreLogger);
    _coreLogger->set_level(spdlog::level::trace);
    _coreLogger->flush_on(spdlog::level::trace);

    _clientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
    spdlog::register_logger(_clientLogger);
    _clientLogger->set_level(spdlog::level::trace);
    _clientLogger->flush_on(spdlog::level::trace);
}
}

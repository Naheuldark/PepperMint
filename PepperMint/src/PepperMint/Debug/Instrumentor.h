#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "PepperMint/Core/Log.h"

namespace PepperMint {

using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult {
    std::string name;

    FloatingPointMicroseconds start;
    std::chrono::microseconds elapsedTime;
    std::thread::id           threadId;
};

struct InstrumentationSession {
    std::string name;

    InstrumentationSession(const std::string& iName) : name(iName) {}
};

class Instrumentor {
  public:
    Instrumentor(const Instrumentor&) = delete;
    Instrumentor(Instrumentor&&)      = delete;

    void beginSession(const std::string& iName, const std::string& iFilepath = "results.json") {
        std::lock_guard lock(_mutex);
        if (_currentSession) {
            // If there is already a current session, then close it before beginning new one.
            // Subsequent profiling output meant for the original session will end up in the newly opened session instead.  That's better than having
            // badly formatted profiling output.
            if (Log::CoreLogger()) { // Edge case: BeginSession() might be before Log::Init()
                PM_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", iName, _currentSession->name);
            }
            internalEndSession();
        }

        _outputStream.open(iFilepath);
        if (_outputStream.is_open()) {
            _currentSession = new InstrumentationSession({iName});
            writeHeader();
        } else {
            if (Log::CoreLogger()) { // Edge case: BeginSession() might be before Log::Init()
                PM_CORE_ERROR("Instrumentor could not open results file '{0}'.", iFilepath);
            }
        }
    }

    void endSession() {
        std::lock_guard lock(_mutex);
        internalEndSession();
    }

    void writeProfile(const ProfileResult& iResult) {
        std::stringstream json;

        json << std::setprecision(3) << std::fixed;
        json << ",{";
        json << "\"cat\":\"function\",";
        json << "\"dur\":" << (iResult.elapsedTime.count()) << ',';
        json << "\"name\":\"" << iResult.name << "\",";
        json << "\"ph\":\"X\",";
        json << "\"pid\":0,";
        json << "\"tid\":" << iResult.threadId << ",";
        json << "\"ts\":" << iResult.start.count();
        json << "}";

        if (_currentSession) {
            _outputStream << json.str();
            _outputStream.flush();
        }
    }

    static Instrumentor& Get() {
        static Instrumentor instance;
        return instance;
    }

  private:
    Instrumentor() : _currentSession(nullptr) {}
    ~Instrumentor() { endSession(); }

    void writeHeader() {
        _outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
        _outputStream.flush();
    }

    void writeFooter() {
        _outputStream << "]}";
        _outputStream.flush();
    }

    // Note: you must already own lock on _mutex before calling InternalEndSession()
    void internalEndSession() {
        if (_currentSession) {
            writeFooter();
            _outputStream.close();
            delete _currentSession;
            _currentSession = nullptr;
        }
    }

  private:
    std::mutex              _mutex;
    InstrumentationSession* _currentSession;
    std::ofstream           _outputStream;
};

class InstrumentationTimer {
  public:
    InstrumentationTimer(const char* iName) : _name(iName), _stopped(false) { _startTimepoint = std::chrono::steady_clock::now(); }

    ~InstrumentationTimer() {
        if (!_stopped) {
            Stop();
        }
    }

  private:
    void Stop() {
        auto endTimepoint = std::chrono::steady_clock::now();
        auto highResStart = FloatingPointMicroseconds{_startTimepoint.time_since_epoch()};
        auto elapsedTime  = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() -
                           std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch();

        Instrumentor::Get().writeProfile({_name, highResStart, elapsedTime, std::this_thread::get_id()});

        _stopped = true;
    }

  private:
    const char*                                        _name;
    std::chrono::time_point<std::chrono::steady_clock> _startTimepoint;
    bool                                               _stopped;
};

namespace InstrumentorUtils {

template <size_t N>
struct ChangeResult {
    char data[N];
};

template <size_t N, size_t K>
constexpr auto cleanupOutputString(const char (&iExpr)[N], const char (&iRemove)[K]) {
    ChangeResult<N> result = {};

    size_t srcIndex = 0;
    size_t dstIndex = 0;
    while (srcIndex < N) {
        size_t matchIndex = 0;
        while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && iExpr[srcIndex + matchIndex] == iRemove[matchIndex])
            matchIndex++;
        if (matchIndex == K - 1)
            srcIndex += matchIndex;
        result.Data[dstIndex++] = iExpr[srcIndex] == '"' ? '\'' : iExpr[srcIndex];
        srcIndex++;
    }

    return result;
}
}
}

#define PM_PROFILE 0
#if PM_PROFILE
// Resolve which function signature macro will be used
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define PM_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define PM_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define PM_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define PM_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define PM_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define PM_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define PM_FUNC_SIG __func__
#else
#define PM_FUNC_SIG "PM_FUNC_SIG unknown!"
#endif

#define PM_PROFILE_BEGIN_SESSION(name, filepath) ::PepperMint::Instrumentor::Get().beginSession(name, filepath)
#define PM_PROFILE_END_SESSION() ::PepperMint::Instrumentor::Get().endSession()
#define HZ_PROFILE_SCOPE_LINE2(name, line)                                                                             \
    constexpr auto                fixedName##line = ::Hazel::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
    ::Hazel::InstrumentationTimer timer##line(fixedName##line.Data)
#define HZ_PROFILE_SCOPE_LINE(name, line) HZ_PROFILE_SCOPE_LINE2(name, line)
#define HZ_PROFILE_SCOPE(name) HZ_PROFILE_SCOPE_LINE(name, __LINE__)
#define PM_PROFILE_FUNCTION() PM_PROFILE_SCOPE(PM_FUNC_SIG)
#else
#define PM_PROFILE_BEGIN_SESSION(name, filepath)
#define PM_PROFILE_END_SESSION()
#define PM_PROFILE_SCOPE(name)
#define PM_PROFILE_FUNCTION()
#endif

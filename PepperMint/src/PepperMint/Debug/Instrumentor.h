#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

namespace PepperMint {

struct ProfileResult {
	const char* name;
	long long start, end;
	size_t threadId;
};

struct InstrumentationSession {
	std::string name;

	InstrumentationSession(const std::string& iName) : name(iName) {}
};

class Instrumentor {
public:
	Instrumentor() : _currentSession(nullptr), _profileCount(0) {}
	~Instrumentor() = default;

	void beginSession(const std::string& iName, const std::string& iFilepath = "results.json") {
		_outputStream.open(iFilepath);
		writeHeader();
		_currentSession = CreateScope<InstrumentationSession>(iName);
	}

	void endSession() {
		writeFooter();
		_outputStream.close();
		_profileCount = 0;
	}

	void writeProfile(const ProfileResult& iResult) {
		if (_profileCount++ > 0)
			_outputStream << ",";

		std::string name = iResult.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		_outputStream << "{";
		_outputStream << "\"cat\":\"function\",";
		_outputStream << "\"dur\":" << (iResult.end - iResult.start) << ',';
		_outputStream << "\"name\":\"" << name << "\",";
		_outputStream << "\"ph\":\"X\",";
		_outputStream << "\"pid\":0,";
		_outputStream << "\"tid\":" << iResult.threadId << ",";
		_outputStream << "\"ts\":" << iResult.start;
		_outputStream << "}";

		_outputStream.flush();
	}

	static Instrumentor& Get() {
		static Instrumentor instance;
		return instance;
	}

private:
	void writeHeader() {
		_outputStream << "{\"otherData\": {},\"traceEvents\":[";
		_outputStream.flush();
	}

	void writeFooter() {
		_outputStream << "]}";
		_outputStream.flush();
	}

private:
	Scope<InstrumentationSession> _currentSession;
	std::ofstream _outputStream;
	int _profileCount;
};

class InstrumentationTimer {
public:
	InstrumentationTimer(const char* iName) :
		_name(iName), _stopped(false) {
		_startTimepoint = std::chrono::high_resolution_clock::now();
	}

	~InstrumentationTimer() {
		if (!_stopped) {
			Stop();
		}
	}

private:
	void Stop() {
		auto&& endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		size_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get().writeProfile({ _name, start, end, threadId });

		_stopped = true;
	}

private:
	const char* _name;
	std::chrono::time_point<std::chrono::high_resolution_clock> _startTimepoint;
	bool _stopped;
};
}

#define PM_PROFILE 1
#if PM_PROFILE
	// Resolve which function signature macro will be used
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define PM_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define PM_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__FUNCSIG__)
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
	#define PM_PROFILE_SCOPE(name) ::PepperMint::InstrumentationTimer timer##__LINE__(name);
	#define PM_PROFILE_FUNCTION() PM_PROFILE_SCOPE(PM_FUNC_SIG)
#else
	#define PM_PROFILE_BEGIN_SESSION(name, filepath)
	#define PM_PROFILE_END_SESSION()
	#define PM_PROFILE_SCOPE(name)
	#define PM_PROFILE_FUNCTION()
#endif 
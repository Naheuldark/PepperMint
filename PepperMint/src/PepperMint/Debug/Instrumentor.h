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
	uint32_t threadId;
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

		uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
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
	#define PM_PROFILE_BEGIN_SESSION(name, filepath) ::PepperMint::Instrumentor::Get().beginSession(name, filepath)
	#define PM_PROFILE_END_SESSION() ::PepperMint::Instrumentor::Get().endSession()
	#define PM_PROFILE_SCOPE(name) ::PepperMint::InstrumentationTimer timer##__LINE__(name);
	#define PM_PROFILE_FUNCTION() PM_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define PM_PROFILE_BEGIN_SESSION(name, filepath)
	#define PM_PROFILE_END_SESSION()
	#define PM_PROFILE_SCOPE(name)
	#define PM_PROFILE_FUNCTION()
#endif 
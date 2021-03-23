#pragma once

#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iomanip>

#include "Hazel/Core/Base.h"

namespace Hazel {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult{
		std::thread::id    ThreadID;
		std::string        Name;
		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;

		ProfileResult(std::thread::id tid, const std::string& name, FloatingPointMicroseconds start, std::chrono::microseconds elapsedTime) :ThreadID(tid), Name(name), Start(start), ElapsedTime(elapsedTime) {};
	};

	struct InstrumentationSession
	{
		std::string Name;
		InstrumentationSession(const std::string &name) :Name(name){};
	};

	class Instrumentor {

	public:
		void BeginSession(const std::string &name, const std::string &filepath = "default.json") {
			// Log::Init() must be before Instrumentor APIs
			HZ_CORE_ASSERT(m_CurrentSession == nullptr, "nested session is currently not supported!");//不支持嵌套session
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_OutputStream.open(filepath);
			if (m_OutputStream.is_open()) {
				m_CurrentSession = new InstrumentationSession({ name });
				writeHeader();
			}
			else {
				HZ_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
			}
		}

		void EndSession() {
			std::lock_guard<std::mutex> lock(m_Mutex);
			internalEndSession();
		}

		void WriteProfile(const ProfileResult& result){
			std::stringstream json;
			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard<std::mutex> lock(m_Mutex);
			if (m_CurrentSession) {
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		static Instrumentor& Get() {
			static Instrumentor instance;
			return instance;
		}

	private:
		Instrumentor() :m_CurrentSession(nullptr) {};

		void writeHeader() {
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void writeFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void internalEndSession() {
			if (m_CurrentSession) {
				writeFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}
	private:
		InstrumentationSession* m_CurrentSession;//注意不是栈/不支持嵌套
		std::ofstream             m_OutputStream;
		std::mutex                       m_Mutex;
	};

	class InstrumentationTimer {
	public:
		InstrumentationTimer(const char* name) :m_Name(name), m_Stopped(false) {
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped) {
				Stop();
			}
		}

		void Stop() {

			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ std::this_thread::get_id(), m_Name, highResStart, elapsedTime });
			m_Stopped = true;
		}
	private:
		const char* m_Name;
		bool m_Stopped;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	};

}


#define HZ_PROFILE 0

// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
	#define HZ_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
	#define HZ_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
	#define HZ_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
	#define HZ_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
	#define HZ_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define HZ_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define HZ_FUNC_SIG __func__
#else
	#define HZ_FUNC_SIG "HZ_FUNC_SIG unknown!"
#endif


#if HZ_PROFILE
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath) ::Hazel::Instrumentor::Get().BeginSession(name, filepath)
	#define HZ_PROFILE_END_SESSION() ::Hazel::Instrumentor::Get().EndSession()
	#define HZ_PROFILE_SCOPE(name) ::Hazel::InstrumentationTimer timer##__LINE__(name);
	#define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(HZ_FUNC_SIG)
#else
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath)
	#define HZ_PROFILE_END_SESSION()
	#define HZ_PROFILE_SCOPE(name)
	#define HZ_PROFILE_FUNCTION()
#endif
#pragma once
// Chrome tracing: chrome://tracing in url bar
// Requires a json file
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
//#define SHOWTIMER
// Formats json file and writes it to a file

namespace Terra {
    struct ProfileResult
    {
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {

    public:
        Instrumentor()
            : m_CurrentSession(nullptr), m_ProfileCount(0)
        {
        }

        // opens files and writes a header, beginning of json file thats needs to be in a particular format for chrome tracing
        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            m_OutputStream.open(filepath);
            WriteHeader();
            m_CurrentSession = new InstrumentationSession{ name };
        }

        // writes footer, closes file
        void EndSession()
        {
            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

        // writes individually timed profile, flushes data in case program crashes
        void WriteProfile(const ProfileResult& result)
        {
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
            m_OutputStream << "}";

            m_OutputStream.flush();
        }

        void WriteHeader()
        {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            m_OutputStream.flush();
        }

        void WriteFooter()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }
    private:
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
    };


    class InstrumentationTimer
    {

    public:
        InstrumentationTimer(const char* name)
            : m_Name(name), m_Stopped(false)
        {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
#ifdef SHOWTIMER
            CORE_TRACE("{0}: {1} ms", m_Name, end - start);
#endif
            // Hash to get thread id as an integer, valid since thread id implements a hash function
            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

            // Createds a ProfileResult struct, which is passed into WriteProfile()
            Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

            m_Stopped = true;
        }

        ~InstrumentationTimer()
        {
            if (!m_Stopped)
                Stop();
        }

    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
        bool m_Stopped;

    };
}
#define TERRA_PROFILE 1
#if TERRA_PROFILE
    #define TERRA_PROFILE_BEGIN_SESSION(name, filepath) ::Terra::Instrumentor::Get().BeginSession(name, filepath)
    #define TERRA_PROFILE_END_SESSION() ::Terra::Instrumentor::Get().EndSession()

// append line number to get a unique name (Timer timer101, Timer time 102, etc.)
    #define TERRA_PROFILE_SCOPE(name) ::Terra::InstrumentationTimer timer##__LINE__(name)

// uses profile scope but with full function signature as the name (ex. void __cdecl Sinebox::Instrumentor::Stop(void) )
// __FUNCTION__ only gives function name
    #define TERRA_PROFILE_FUNCTION() TERRA_PROFILE_SCOPE(__FUNCSIG__)

#else
    #define TERRA_PROFILE_BEGIN_SESSION(name, filepath)
    #define TERRA_PROFILE_END_SESSION()
    #define TERRA_PROFILE_SCOPE(name)
    #define TERRA_PROFILE_FUNCTION()

#endif
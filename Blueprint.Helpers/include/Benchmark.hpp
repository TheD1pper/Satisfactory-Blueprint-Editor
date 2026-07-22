#pragma once

#include <chrono>
#include <fstream>
#include <thread>
#include <filesystem>

// Slightly modified header-only version of the benchmarker by the one and only The Cherno!
// Love you Cherno <3

namespace Benchmark
{
#pragma region Instrumentation
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

    inline std::filesystem::path DefaultSavePath{};

    inline void OverwriteSavePath(const std::filesystem::path& _Path)
    {
        DefaultSavePath = _Path;
    }

    class Instrumentor
    {
    private:
        InstrumentationSession* CurrentSession;
        std::ofstream OutputStream;
        int ProfileCount;

    public:
        inline Instrumentor()
            : CurrentSession(nullptr), ProfileCount(0)
        {
        }

        inline void BeginSession(const std::string& _Name, const std::string& _Filepath = DefaultSavePath.string() + "benchmark.json")
        {
            OutputStream.open(_Filepath);
            WriteHeader();
            CurrentSession = new InstrumentationSession{ _Name };
        }

        inline void EndSession()
        {
            WriteFooter();
            OutputStream.close();
            delete CurrentSession;
            CurrentSession = nullptr;
            ProfileCount = 0;
        }

        inline void WriteProfile(const ProfileResult& _Result)
        {
            if (ProfileCount++ > 0)
                OutputStream << ",";

            std::string name = _Result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            OutputStream << "{";
            OutputStream << "\"cat\":\"function\",";
            OutputStream << "\"dur\":" << (_Result.End - _Result.Start) << ',';
            OutputStream << "\"name\":\"" << name << "\",";
            OutputStream << "\"ph\":\"X\",";
            OutputStream << "\"pid\":0,";
            OutputStream << "\"tid\":" << _Result.ThreadID << ",";
            OutputStream << "\"ts\":" << _Result.Start;
            OutputStream << "}";

            OutputStream.flush();
        }

        inline void WriteHeader()
        {
            OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            OutputStream.flush();
        }

        inline void WriteFooter()
        {
            OutputStream << "]}";
            OutputStream.flush();
        }

        inline ~Instrumentor()
        {
            EndSession();
        }

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }
    };
#pragma endregion Everything related to json formatting

#pragma region Timer
    class InstrumentationTimer
    {
    private:
        std::string Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> StartTimepoint;
        bool Stopped;

    public:
        inline InstrumentationTimer(const std::string& _Name)
            : Name(_Name),
            Stopped(false)
        {
            StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        inline ~InstrumentationTimer()
        {
            if (!Stopped)
                Stop();
        }

        inline void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
            Instrumentor::Get().WriteProfile({ Name, start, end, threadID });

            Stopped = true;
        }
    };

#define TRACING 1
#if TRACING
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define BENCH_FUNC() \
    Benchmark::InstrumentationTimer CONCAT(FunctionTimer, __LINE__)(__FUNCSIG__)
#define BENCH_SCOPE(_Name) \
    Benchmark::InstrumentationTimer CONCAT(ScopeTimer, __LINE__)(_Name)

#else
#define BENCH_FUNC()
#define BENCH_SCOPE(_Name)
#endif

#pragma endregion Timing stuff and etc idk
}
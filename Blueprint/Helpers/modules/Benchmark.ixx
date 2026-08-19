module;

export module Helpers.Benchmark;

import <chrono>;
import <fstream>;
import <thread>;
import <filesystem>;

// Slightly modified module version of the benchmarker by the one and only The Cherno!
// Love you lad <3

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

    std::filesystem::path DefaultSavePath{};

    export void OverwriteSavePath(const std::filesystem::path& _Path)
    {
        DefaultSavePath = _Path;
    }

    export class Instrumentor
    {
    private:
        InstrumentationSession* CurrentSession;
        std::ofstream OutputStream;
        int ProfileCount;

    public:
        Instrumentor()
            : CurrentSession(nullptr), ProfileCount(0)
        {
        }

        void BeginSession(const std::string& _Name, const std::filesystem::path& _Filepath = DefaultSavePath / "benchmark.json")
        {
            OutputStream.open(_Filepath);
            WriteHeader();
            CurrentSession = new InstrumentationSession{ _Name };
        }

        void EndSession()
        {
            WriteFooter();
            OutputStream.close();
            delete CurrentSession;
            CurrentSession = nullptr;
            ProfileCount = 0;
        }

        void WriteProfile(const ProfileResult& _Result)
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

        void WriteHeader()
        {
            OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            OutputStream.flush();
        }

        void WriteFooter()
        {
            OutputStream << "]}";
            OutputStream.flush();
        }

        ~Instrumentor()
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
    export class InstrumentationTimer
    {
    private:
        std::string Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> StartTimepoint;
        bool Stopped;

    public:
        InstrumentationTimer(const std::string& _Name)
            : Name(_Name),
            Stopped(false)
        {
            StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!Stopped)
                Stop();
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
            Instrumentor::Get().WriteProfile({ Name, start, end, threadID });

            Stopped = true;
        }
    };

#pragma endregion The timer class
}
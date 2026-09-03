module;
module Helpers.Benchmark;

import std;

namespace fs = std::filesystem;

namespace Benchmark
{
#pragma region Instrumentation
	Instrumentor::Instrumentor()
        : CurrentSession(nullptr), ProfileCount(0)
    {
    }

    void Instrumentor::BeginSession(const std::string& _Name, const std::filesystem::path& _Filepath)
    {
        OutputStream.open(_Filepath);
        WriteHeader();
        CurrentSession = new InstrumentationSession{ _Name };
    }

    void Instrumentor::EndSession()
    {
        WriteFooter();
        OutputStream.close();
        delete CurrentSession;
        CurrentSession = nullptr;
        ProfileCount = 0;
    }

    void Instrumentor::WriteProfile(const ProfileResult& _Result)
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

    void Instrumentor::WriteHeader()
    {
        OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        OutputStream.flush();
    }

    void Instrumentor::WriteFooter()
    {
        OutputStream << "]}";
        OutputStream.flush();
    }

    Instrumentor::~Instrumentor()
    {
        EndSession();
    }

    Instrumentor& Instrumentor::Get()
    {
        static Instrumentor instance;
        return instance;
    }

    void OverwriteSavePath(const std::filesystem::path& _Path)
    {
        DefaultSavePath = _Path;
    }
#pragma endregion Everything related to json formatting

#pragma region Timer
    InstrumentationTimer::InstrumentationTimer(const std::string& _Name)
        : Name(_Name),
        Stopped(false)
    {
        StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    InstrumentationTimer::~InstrumentationTimer()
    {
        if (!Stopped)
            Stop();
    }

    void InstrumentationTimer::Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        uint32_t threadID = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
        Instrumentor::Get().WriteProfile({ Name, start, end, threadID });

        Stopped = true;
    }
#pragma endregion The timer class
}
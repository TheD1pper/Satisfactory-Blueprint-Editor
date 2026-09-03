module;
export module Helpers.Benchmark;

import std.compat;

// Slightly modified module version of the benchmarker by the one and only The Cherno!
// Love you lad <3

namespace fs = std::filesystem;

namespace Benchmark
{
#pragma region Instrumentation
    fs::path DefaultSavePath{};

    struct ProfileResult
    {
        std::string Name;
        int64_t Start;
        int64_t End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };


    export class Instrumentor
    {
    public:
        Instrumentor();

        void BeginSession(const std::string& _Name, const fs::path& _Filepath = DefaultSavePath / "benchmark.json");
        void EndSession();
        void WriteProfile(const ProfileResult& _Result);
        void WriteHeader();
        void WriteFooter();
        ~Instrumentor();
        static Instrumentor& Get();

    private:
        InstrumentationSession* CurrentSession;
        std::ofstream OutputStream;
        int ProfileCount;
    };

    export void OverwriteSavePath(const fs::path& _Path);
#pragma endregion Everything related to json formatting

#pragma region Timer
    export class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const std::string& _Name);
        ~InstrumentationTimer();
        void Stop();

    private:
        std::string Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> StartTimepoint;
        bool Stopped;
    };

#pragma endregion The timer class
}
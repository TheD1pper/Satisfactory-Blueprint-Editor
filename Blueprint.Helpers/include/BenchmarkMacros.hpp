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
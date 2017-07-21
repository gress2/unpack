#include "benchmark/benchmark_api.h"
#include "benchmark_utils.hpp"
#include "unpack.hpp"

static void DoBenchmark(benchmark::State& state, unpack_benchmark::opts o) {
    while (state.KeepRunning()) {
        state.PauseTiming();
        auto start_timing = [&state]() { state.ResumeTiming(); };
        std::cout << unpack_benchmark::run_benchmark(o, start_timing) << std::endl;
    }
}

int main(int argc, char** argv) {
    assert(argc == 8);
    benchmark::RegisterBenchmark("DoBenchmark", DoBenchmark, unpack_benchmark::opts(argv));
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}

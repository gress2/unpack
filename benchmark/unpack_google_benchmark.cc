#include "benchmark/benchmark_api.h"
#include "benchmark_utils.hpp"
#include "unpack.hpp"

static void DoBenchmark(benchmark::State& state, unpack_benchmark::opts o) {
  while (state.KeepRunning()) {
    state.PauseTiming();
    auto start_timing = [&state]() { state.ResumeTiming(); };
    unpack_benchmark::run_benchmark(o, start_timing);
  }
}

int main(int argc, char** argv) {
  assert(argc >= 8);
  auto opts = unpack_benchmark::opts(argv, argc);
  benchmark::RegisterBenchmark("DoBenchmark", [&opts](auto& state) { DoBenchmark(state, opts); })->MinTime(0.01);
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}

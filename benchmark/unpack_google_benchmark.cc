#include "benchmark/benchmark_api.h"
#include "benchmark_utils.hpp"
#include "unpack.hpp"

static void AosFullSuite(benchmark::State& state) {
    while (state.KeepRunning()) {
        using type = std::vector<std::tuple<int, double, double>>;
        const std::size_t count = 1 << 20;
        volatile unsigned char tmp_aos = 0;
        state.PauseTiming();
        auto aos = unpack_benchmark::make_random_aos<type>(count);
        auto doubling = [](auto&& v) {
            for (size_t i = 0; i < 1000; i++) {
                for (auto&& element : v) {
                    std::get<0>(element) *= 2;
                }
            }
        };
        state.ResumeTiming();
        unpack_benchmark::benchmark(doubling, aos);
        benchmark::DoNotOptimize(
            unpack_benchmark::check_bytes(std::begin(aos), std::end(aos))
        );
    }
}

static void SoaFullSuite(benchmark::State& state) {
    while (state.KeepRunning()) {
        using type = std::vector<unpack<std::tuple<int, double, double>>>;
        const std::size_t count = 1 << 20;
        state.PauseTiming();
        auto soa = unpack_benchmark::make_random_soa<type>(count);
        auto doubling = [](auto&& v) {
            for (size_t i = 0; i < 1000; i++) {
                for (auto&& element : v) {
                    std::get<0>(element) *= 2;
               }
            }
        };
        state.ResumeTiming();
        unpack_benchmark::benchmark(doubling, soa);
        benchmark::DoNotOptimize(
            unpack_benchmark::check_bytes(std::begin(soa), std::end(soa))
        );
    }
}

BENCHMARK(AosFullSuite);
BENCHMARK(SoaFullSuite);

BENCHMARK_MAIN();

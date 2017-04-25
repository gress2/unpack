#include "benchmark/benchmark_api.h"
#include "unpack.hpp"

static void VectorConstruction(benchmark::State& state) {
    while (state.KeepRunning()) {
        std::vector<std::tuple<int, float>> v;
    }
}

static void UnpackVectorConstruction(benchmark::State& state) {
    while (state.KeepRunning()) {
        std::vector<unpack<std::tuple<int, float>>> v; 
    }
}

static void VectorPushBack(benchmark::State& state) {
    while (state.KeepRunning()) {
        std::vector<std::tuple<int, float>> v;
        v.push_back(std::make_tuple(3, 4));
    }
}

static void UnpackVectorPushBack(benchmark::State& state) {
    while (state.KeepRunning()) {
        std::vector<unpack<std::tuple<int, float>>> v;
        v.push_back(std::make_tuple(3, 4));
    }
}

static void UnpackVectorIteration(benchmark::State& state) {
    while (state.KeepRunning()) {
        state.PauseTiming();
        std::vector<unpack<std::tuple<int, int, int, int, int, int, int, int>>> v;
        for (size_t i = 0; i < 1000000; i++) {
            v.push_back(std::make_tuple(i, i, i, i, i, i, i, i));
        }
        state.ResumeTiming();
        for (auto it = v.begin(); it != v.end(); ++it) {
            std::get<0>(*it);
        }
    }
}

static void VectorIteration(benchmark::State& state) {
    while (state.KeepRunning()) {
        state.PauseTiming();
        std::vector<std::tuple<int, int, int, int, int, int, int, int>> v;
        for (size_t i = 0; i < 1000000; i++) {
            v.push_back(std::make_tuple(i, i, i, i, i, i, i, i));
        }
        state.ResumeTiming();
        for (auto it = v.begin(); it != v.end(); ++it) {
            std::get<0>(*it);
        }
    }
}

BENCHMARK(VectorConstruction);
BENCHMARK(UnpackVectorConstruction);
BENCHMARK(VectorPushBack);
BENCHMARK(UnpackVectorPushBack);
BENCHMARK(VectorIteration);
BENCHMARK(UnpackVectorIteration);

BENCHMARK_MAIN();

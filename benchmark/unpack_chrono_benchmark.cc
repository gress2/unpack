// Compilation: g++ -Wall -Wextra -pedantic -std=c++14 -O3 unpack_benchmark.cpp
// -o unpack_benchmark
#include <cassert>

#include "benchmark_utils.hpp"
#include "unpack.hpp"

int main(int argc, char* argv[]) {
  assert(argc == 8);
  unpack_benchmark::opts o(argv, argc);

  typename std::chrono::high_resolution_clock::time_point tbegin;
  typename std::chrono::high_resolution_clock::time_point tend;
  std::chrono::duration<double> duration;

  auto start_timing = [&tbegin]() {
    tbegin = std::chrono::high_resolution_clock::now();
  };

  std::cout << unpack_benchmark::run_benchmark(o, start_timing) << std::endl;
  tend = std::chrono::high_resolution_clock::now();
  duration = tend - tbegin;
  std::cout << duration.count() << std::endl;
}

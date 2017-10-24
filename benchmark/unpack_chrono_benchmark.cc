// Compilation: g++ -Wall -Wextra -pedantic -std=c++14 -O3 unpack_benchmark.cpp
// -o unpack_benchmark
#include <cassert>
#include <cmath>
#include "benchmark_utils.hpp"
#include "unpack.hpp"

template <class Opts>
double run(Opts&& opts) {
  typename std::chrono::high_resolution_clock::time_point tbegin;
  typename std::chrono::high_resolution_clock::time_point tend;
  std::chrono::duration<double> duration;
  volatile double tmp = 0;
  auto start_timing = [&tbegin]() {
    tbegin = std::chrono::high_resolution_clock::now();
  };
  tmp = unpack_benchmark::run_benchmark(std::forward<Opts>(opts), start_timing);
  if (std::isnan(tmp)) {
    std::cerr << tmp << std::endl;
  }
  tend = std::chrono::high_resolution_clock::now();
  duration = tend - tbegin;
  return duration.count();
}

int main(int argc, char* argv[]) {
  assert(argc >= 9);
  unpack_benchmark::opts o(argv, argc);

  typename std::chrono::high_resolution_clock::time_point tbegin;
  std::chrono::duration<double> duration;

  if (o.time_limit > 0) {
    tbegin = std::chrono::high_resolution_clock::now();
    while (duration.count() < o.time_limit) {
      std::cout << run(o) << std::endl;
      duration = std::chrono::high_resolution_clock::now() - tbegin;
    }
  } else {
    std::cout << run(o) << std::endl;
  }

  return 0;
}

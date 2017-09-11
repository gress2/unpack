#include <cassert>
#include <iostream>
#include <typeinfo>

#include "benchmark_utils.hpp"
#include "unpack.hpp"

int main(int argc, char* argv[]) {
  assert(argc == 8);
  unpack_benchmark::opts o(argv, argc);
  auto start_timing = []() {}; // no timing functionality
  std::cout << unpack_benchmark::run_benchmark(o, start_timing) << std::endl;
}

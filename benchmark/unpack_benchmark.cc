#include <cassert>
#include <typeinfo>

#include "benchmark_utils.hpp"
#include "unpack.hpp"

int main(int argc, char* argv[]) {
    assert(argc == 8); 
    unpack_benchmark::opts o(argv);
    auto resume = []() {};
    unpack_benchmark::run_benchmark(o, resume);
}

#include <cassert>
#include <typeinfo>

#include "benchmark_utils.hpp"
#include "unpack.hpp"

int main(int argc, char* argv[]) {
    assert(argc == 8); 
    unpack_benchmark::opts o(argv);
    run_benchmark(o);
}

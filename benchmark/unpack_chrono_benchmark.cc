// Compilation: g++ -Wall -Wextra -pedantic -std=c++14 -O3 unpack_benchmark.cpp
// -o unpack_benchmark
#include "benchmark_utils.hpp"
#include "unpack.hpp"

int main(int argc, char* argv[])
{
    const std::size_t count = argc > 1 ? std::stoull(argv[1]) : 1 << 20;
    volatile unsigned char tmp_aos = 0;
    volatile unsigned char tmp_soa = 0;
    auto aos = unpack_benchmark::make_random_aos<std::vector<std::tuple<int, double, double>>>(count);
    auto soa = unpack_benchmark::make_random_soa<std::vector<unpack<std::tuple<int, double, double>>>>(count);
    auto doubling = [](auto&& v){
        for (size_t i = 0; i < 1000; i++) {
            for (auto&& element : v) {
                std::get<0>(element) *= 2;
            }
        }
    };
    std::cout << unpack_benchmark::benchmark(doubling, aos) << std::endl;
    tmp_aos = unpack_benchmark::check_bytes(std::begin(aos), std::end(aos));
    std::cout << unpack_benchmark::benchmark(doubling, soa) << std::endl;
    tmp_soa = unpack_benchmark::check_bytes(std::begin(soa), std::end(soa));
    return tmp_aos + tmp_soa;
}

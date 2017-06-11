#include <cassert>
#include <typeinfo>

#include "benchmark_utils.hpp"
#include "unpack.hpp"

void AosFullSuite() {
    using type = std::vector<std::tuple<int, double, double>>;
    const std::size_t count = 1 << 20; 
    volatile unsigned char tmp_aos = 0;
    auto aos = unpack_benchmark::make_random_aos<type>(count);
    auto doubling = [](auto&& v) {
        for (size_t i = 0; i < 1000; i++) {
            for (auto&& element : v) {
                std::get<0>(element) *= 2;
            }
        }
    };
    unpack_benchmark::benchmark(doubling, aos); 
    tmp_aos = unpack_benchmark::check_bytes(std::begin(aos), std::end(aos));
    std::cout << tmp_aos << std::endl;
}

void SoaFullSuite() {
    using type = std::vector<unpack<std::tuple<int, double, double>>>;
    const std::size_t count = 1 << 20;
    volatile unsigned char tmp_soa = 0;
    auto soa = unpack_benchmark::make_random_soa<type>(count);
    auto doubling = [](auto&& v) {
        for (size_t i = 0; i < 1000; i++) {
            for (auto&& element : v) {
                std::get<0>(element) *= 2;
            }
        }
    };
    unpack_benchmark::benchmark(doubling, soa);
    tmp_soa = unpack_benchmark::check_bytes(std::begin(soa), std::end(soa));
    std::cout << tmp_soa << std::endl;
}

int main(int argc, char* argv[]) {
    assert(argc == 7); 
    unpack_benchmark::opts o(argv);
}

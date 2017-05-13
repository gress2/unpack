// Author: Vincent Reverdy
// Benchmarking unpacking strategies
// Compilation: g++ -Wall -Wextra -pedantic -std=c++14 -O3 unpack_benchmark.cpp
// -o unpack_benchmark

// Preamble
#include "unpack.hpp"
#include <array>
#include <chrono>
#include <limits>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

// Xorshift random engine
struct xorshift_engine
{
    using type = unsigned int;
    using result_type = type;
    constexpr xorshift_engine(result_type seed = 0) noexcept
    : _state(seed ? seed : !seed) {
    }
    static constexpr result_type min() noexcept {
        return std::numeric_limits<result_type>::min();
    }
    static constexpr result_type max() noexcept {
        return std::numeric_limits<result_type>::max();
    }
    constexpr result_type operator()() noexcept {
        constexpr std::size_t shift0 = 13;
        constexpr std::size_t shift1 = 17;
        constexpr std::size_t shift2 = 5;
        _state ^= _state << shift0;
        _state ^= _state >> shift1;
        _state ^= _state << shift2;
        return _state;
    }
    private: type _state;
};

// Generic uniform random distribution type
template <class T>
using uniform_distribution = std::conditional_t<
    std::is_floating_point<T>::value,
    std::uniform_real_distribution<
        std::conditional_t<std::is_floating_point<T>::value, T, double>
    >,
    std::uniform_int_distribution<
        std::conditional_t<std::is_integral<T>::value, T, int>
    >
>;

// Make a random array of structures
template <class T>
std::vector<T> make_random_aos(
    std::size_t count,
    std::size_t min = std::numeric_limits<std::size_t>::min(),
    std::size_t max = std::numeric_limits<std::size_t>::max(),
    std::size_t seed = std::random_device()()
) {
    xorshift_engine engine(seed);
    uniform_distribution<std::size_t> distribution(min, max);
    std::vector<T> vector(count);
    T tmp;
    for (std::size_t i = 0; i < count; ++i) {
        tuple_for_each([&distribution, &engine](auto&& element){
            element = distribution(engine);
        }, tmp);
        vector[i] = tmp;
    }
    return vector;
}

// Make a random structure of arrays
template <class T>
std::vector<T> make_random_soa(
    std::size_t count,
    std::size_t min = std::numeric_limits<std::size_t>::min(),
    std::size_t max = std::numeric_limits<std::size_t>::max(),
    std::size_t seed = std::random_device()()
) {
    xorshift_engine engine(seed);
    uniform_distribution<std::size_t> distribution(min, max);
    std::vector<T> vector(count);
    for (std::size_t i = 0; i < count; ++i) {
        tuple_for_each([&distribution, &engine](auto&& element){
            element = distribution(engine);
        }, vector[i]);
    }
    return vector;
}

// Benchmark a function
template <class F, class... Args>
double benchmark(F&& f, Args&&... args)
{
    typename std::chrono::high_resolution_clock::time_point tbegin;
    typename std::chrono::high_resolution_clock::time_point tend;
    std::chrono::duration<double> duration;
    tbegin = std::chrono::high_resolution_clock::now();
    std::forward<F>(f)(std::forward<Args>(args)...);
    tend = std::chrono::high_resolution_clock::now();
    duration = tend - tbegin;
    return duration.count();
}

// Check the bytes of the result
template <class It>
unsigned char check_bytes(It first, It last) {
    using value_type = typename std::iterator_traits<It>::value_type;
    using byte_type = unsigned char;
    byte_type byte = 0;
    auto accumulator = [](const byte_type& x, const value_type& y) {
        auto checker = [](const value_type& x) {
            const byte_type* first = reinterpret_cast<const byte_type*>(&x);
            const byte_type* last = first + sizeof(x);
            return std::accumulate(first, last, static_cast<byte_type>(0));
        };
        return x + checker(y);
    };
    return std::accumulate(first, last, byte, accumulator);
}

// Main function
int main(int argc, char* argv[])
{
    using type = std::tuple<int, double, double, double, double, double, double, double, double, double, double, double, double>;
    const std::size_t count = argc > 1 ? std::stoull(argv[1]) : 1 << 20;
    volatile unsigned char tmp_aos = 0;
    volatile unsigned char tmp_soa = 0;
    auto aos = make_random_aos<type>(count);
    auto soa = make_random_soa<unpack<type>>(count);
    auto doubling = [](auto&& v){
        for (size_t i = 0; i < 1000; i++) {
            for (auto&& element : v) {
                std::get<0>(element) *= 2;
            }
        }
    };
//    std::cout<<benchmark(doubling, aos)<<std::endl;
    tmp_aos = check_bytes(std::begin(aos), std::end(aos));
    std::cout<<benchmark(doubling, soa)<<std::endl;
    tmp_soa = check_bytes(std::begin(soa), std::end(soa));
    return tmp_aos + tmp_soa;
}

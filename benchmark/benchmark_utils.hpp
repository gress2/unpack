#ifndef BENCHMARK_UTILS
#define BENCHMARK_UTILS

// Author: Vincent Reverdy
// Benchmarking unpacking strategies
#include "unpack.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <deque>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <random>
#include <string>
#include <thread>
#include <vector>

namespace unpack_benchmark
{

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
// TODO: allocator
template <typename T>
T make_random_aos(
    std::size_t count,
    std::size_t min = std::numeric_limits<std::size_t>::min(),
    std::size_t max = std::numeric_limits<std::size_t>::max(),
    std::size_t seed = std::random_device()()
) {
    xorshift_engine engine(seed);
    uniform_distribution<std::size_t> distribution(min, max);
    T container(count);
    typename T::value_type tmp;
    for (std::size_t i = 0; i < count; ++i) {
        tuple_for_each([&distribution, &engine](auto&& element){
            element = distribution(engine);
        }, tmp);
        container[i] = tmp;
    }
    return container;
} 
// TODO: allocator
template <typename T>
T make_random_soa(
    std::size_t count,
    std::size_t min = std::numeric_limits<std::size_t>::min(),
    std::size_t max = std::numeric_limits<std::size_t>::max(),
    std::size_t seed = std::random_device()()
) {
    xorshift_engine engine(seed);
    uniform_distribution<std::size_t> distribution(min, max);
    T container(count);
    for (std::size_t i = 0; i < count; ++i) {
        tuple_for_each([&distribution, &engine](auto&& element){
            element = distribution(engine);
        }, container[i]);
    }
    return container;
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

using type0 = typename std::tuple<int>;
using type1 = typename std::tuple<int, double, double>;
using type2 = typename std::tuple<int, int, unsigned char>; 

template <typename T>
auto simple(T&& v) {
    for (size_t i = 0; i< 1000; i++) {
        for (auto&& element : v) {
            std::get<0>(element) *= 2;
        }
    }
}

template <typename T>
auto complex(T&& v) {
    for (size_t i = 0; i < 1000; i++) {
        for (auto&& element : v) {
            std::get<0>(element) += 2;
        }
    }
}
 
struct opts {
    enum data_structure { aos, soa };
    enum complexity { simple, complex };
    enum access_pattern { independent, single, combined };
    enum container_type { vector, list, deque };

    data_structure ds;  
    container_type ct;
    int container_size;
    int type_index;
    complexity cmp;
    access_pattern ap;

    opts(char* argv[]) {
        if (strncmp(argv[1], "aos", 3) == 0) {
            ds = opts::data_structure::aos; 
        } else if (strncmp(argv[1], "soa", 3) == 0) {
            ds = opts::data_structure::soa;
        } else {
            throw std::runtime_error("Invalid data structure provided");
        } 

        if (strncmp(argv[2], "vector", 10) == 0) {
            ct = opts::container_type::vector;
        } else if (strncmp(argv[2], "list", 10) == 0) {
            ct = opts::container_type::list;
        } else if (strncmp(argv[2], "deque", 10) == 0) {
            ct = opts::container_type::deque;
        } else {
            throw std::runtime_error("Invalid container type provided"); 
        }

        container_size = std::stoi(argv[3]);        
        
        type_index = std::stoi(argv[4]);
        if (type_index < 0 || type_index > 40) {
            throw std::runtime_error("Invalid type index provided");
        }

        if (strncmp(argv[5], "simple", 10) == 0) {
            cmp = opts::complexity::simple; 
        } else if (strncmp(argv[5], "complex", 10) == 0) {
            cmp = opts::complexity::complex;
        } else {
            throw std::runtime_error("Invalid complexity provided");
        } 

        if (strncmp(argv[6], "independent", 15) == 0) {
            ap = opts::access_pattern::independent;
        } else if (strncmp(argv[6], "single", 15) == 0) {
            ap = opts::access_pattern::single;
        } else if (strncmp(argv[6], "combined", 15) == 0) {
            ap = opts::access_pattern::combined;
        } else {
            throw std::runtime_error("Invalid access pattern provided");
        }
    }
};

template <typename T>
auto run_fn(T&& t) {

}

template <typename T>
struct type_map {
    using vector_aos = std::vector<T>;
    using vector_soa = std::vector<unpack<T>>;
    // TODO: fix make_random_* functions to support lists
    using list_aos = std::vector<T>;
    using list_soa = std::vector<unpack<T>>;
    using deque_aos = std::deque<T>;
    using deque_soa = std::deque<T>;
};

template <typename T>
void dispatch(opts& _opts) {
   switch (_opts.ct) {
       case opts::container_type::vector:
           if (_opts.ds == opts::data_structure::aos) {
               run_fn(make_random_aos<typename T::vector_aos>(_opts.container_size));
           } else {
               run_fn(make_random_soa<typename T::vector_soa>(_opts.container_size));
           }
       case opts::container_type::list:
           if (_opts.ds == opts::data_structure::aos) {
               run_fn(make_random_aos<typename T::list_aos>(_opts.container_size));
           } else {
               run_fn(make_random_soa<typename T::list_soa>(_opts.container_size));
           }
       case opts::container_type::deque:
           if (_opts.ds == opts::data_structure::aos) {
               run_fn(make_random_aos<typename T::deque_aos>(_opts.container_size));
           } else {
               run_fn(make_random_soa<typename T::deque_soa>(_opts.container_size));
           }
   } 
}

void run_benchmark(opts& _opts) {
    switch (_opts.type_index) {
        case 0:
            dispatch<type_map<type0>>(_opts);
            break;
        case 1: 
            dispatch<type_map<type1>>(_opts);
            break;
        case 2: 
            dispatch<type_map<type2>>(_opts);
    }
}

}
#endif

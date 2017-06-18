#ifndef BENCHMARK_UTILS
#define BENCHMARK_UTILS

// Author: Vincent Reverdy
// Benchmarking unpacking strategies
#include "unpack.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
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

template <typename T>
T convert_size_t(std::size_t n) {
    return T(n); 
}

template <>
std::string convert_size_t<std::string>(std::size_t n) {
    return std::to_string(n);
} 

template <typename It>
void fill_container_randomly(
    It&& begin,
    It&& end,
    std::size_t min = std::numeric_limits<std::size_t>::min(),
    std::size_t max = std::numeric_limits<std::size_t>::max(),
    std::size_t seed = std::random_device()()
) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::size_t i = 0;
    for (auto&& it = begin; it != end; ++it) {
        tuple_for_each([&i](auto&& element) {
            element = convert_size_t<decltype(element)>(i++);
        }, *it); 
    }
    std::shuffle(begin, end, g);
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
using type1 = typename std::tuple<int, int, int, int, int>;
using type2 = typename std::tuple<int, double, double>;
using type3 = typename std::tuple<int, int, int, int, int>;
using type4 = typename std::tuple<int, int>;

struct opts {
    enum data_structure { aos, soa };
    enum access_pattern { independent, single, combined };
    enum container_type { vector, list, deque };
    enum _function { simple, complex };

    data_structure ds;
    container_type ct;
    int container_size;
    int type_index;
    _function fn;
    access_pattern ap;

    // example: ./Release/bin/unpack_benchmark soa vector 1000000 1 simple independent
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
            fn = opts::_function::simple;
        } else if (strncmp(argv[5], "complex", 10) == 0) {
            fn = opts::_function::complex;
        } else {
            throw std::runtime_error("Invalid function provided");
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
void simple_single(T& container) {
    for (size_t i = 0; i < 1000; i++) {
        for (auto&& element : container) {
            std::get<0>(element) *= 2;
        } 
    }
}

template <typename T> 
typename std::enable_if<std::is_integral<typename std::remove_reference<T>::type>
             ::value>::type complex_one_value_fn(T&& t) {
	if (t != 2) {
        if (t < 2 || t % 2 == 0) {
            t *= 2;
        }
        for (int i = 3; (i * i) <= t; i += 2){
            if (t % i == 0 ) {
                t *= 2;
            }
        }
    }
    t *= 4;    
}

template <typename T> 
typename std::enable_if<std::is_floating_point<typename std::remove_reference<T>::type>
             ::value>::type complex_one_value_fn(T&& t) {
    t = cos(t) + sin(t) + tan(t);		
}

template <typename T>
void complex_single(T& container) {
    for (size_t i = 0; i < 1000; i++) {
        for (auto&& element : container) {
            complex_one_value_fn(std::get<0>(element));
        } 
    }
}

template <typename T>
void simple_independent(T& container) {
    for (size_t i = 0; i < 1000; i++) {
        for (auto&& element : container) {
            tuple_for_each([](auto&& tuple_elem) {
               tuple_elem *= 2;
            }, std::forward<decltype(element)>(element));
        }
    }
}

template <typename T>
void complex_independent(T& container) {
    for (size_t i = 0; i < 1000; i++) {
        for (auto&& element : container) {
            tuple_for_each([](auto&& tuple_elem) {
			   complex_one_value_fn(std::forward
			       <decltype(tuple_elem)>(tuple_elem));
            }, std::forward<decltype(element)>(element));
        }
    }
}

double product() {
    return 1;
}

template <typename T, typename... Tail>
auto product(T&& t, Tail&&... tail) {
    return t * product(std::forward<Tail>(tail)...);
}

template <typename T, size_t... Indices>
auto simple_combined_fn_impl(T&& t, std::index_sequence<Indices...>) {
    return product(std::get<Indices>(t)...);
} 

template <typename T>
void simple_combined_fn(T&& t) {
    constexpr auto size = std::tuple_size<typename std::remove_reference<T>::type>{};
    auto fn_res = simple_combined_fn_impl(std::forward<T>(t), 
                    std::make_index_sequence<size>{}); 
    tuple_for_each([&fn_res](auto&& tuple_elem) {
        tuple_elem += fn_res;
    }, std::forward<T>(t));
}

template <typename T>
void simple_combined(T& container) {
    for (size_t i = 0; i < 1000; i++) {
        for (auto&& element : container) {
            simple_combined_fn(std::forward<decltype(element)>(element));
        }
    }
}

double l4_norm() {
    return 0;
}

template <typename T, typename... Tail>
auto l4_norm(T&& t, Tail&&... tail) {
    return pow(std::forward<T>(t), 4) + l4_norm(std::forward<Tail>(tail)...);
}

template <typename T, size_t... Indices>
auto complex_combined_fn_impl(T&& t, std::index_sequence<Indices...>) {
    return pow(l4_norm(std::get<Indices>(t)...), 1/4.0);
} 

template <typename T>
void complex_combined_fn(T&& t) {
    constexpr auto size = std::tuple_size<typename std::remove_reference<T>::type>{};
    auto fn_res = complex_combined_fn_impl(std::forward<T>(t), 
                    std::make_index_sequence<size>{}); 
    tuple_for_each([&fn_res](auto&& tuple_elem) {
        tuple_elem += fn_res;
    }, std::forward<T>(t));
}

template <typename T>
void complex_combined(T& container) {
    for (size_t i = 0; i < 1000; i++) {
        for (auto&& element : container) {
            complex_combined_fn(std::forward<decltype(element)>(element));
        }
    }
}

template <typename T>
void run_fn(opts& _opts) {
    T container(_opts.container_size);
    fill_container_randomly(container.begin(), container.end());
    if (_opts.fn == opts::_function::simple) {
        switch(_opts.ap) {
            case opts::access_pattern::single:
                simple_single(container); 
                break;
            case opts::access_pattern::independent:
                simple_independent(container); 
                break;
            case opts::access_pattern::combined:
                simple_combined(container);
                break;
        }
    } else {
        switch(_opts.ap) {
            case opts::access_pattern::single:
                complex_single(container);
                break;
            case opts::access_pattern::independent:
                complex_independent(container);
                break;
            case opts::access_pattern::combined:
                complex_combined(container);
                break;
        } 
    }
}

template <typename T>
struct type_map {
    using vector_aos = std::vector<T>;
    using vector_soa = std::vector<unpack<T>>;
    using list_aos = std::vector<T>;
    using list_soa = std::vector<unpack<T>>;
    using deque_aos = std::vector<T>;
    using deque_soa = std::vector<unpack<T>>;
};

template <typename T>
void dispatch(opts& _opts) {
   switch (_opts.ct) {
       case opts::container_type::vector:
           if (_opts.ds == opts::data_structure::aos) {
               run_fn<typename T::vector_aos>(_opts);
           } else {
               run_fn<typename T::vector_soa>(_opts);
           }
           break;
       case opts::container_type::list:
           if (_opts.ds == opts::data_structure::aos) {
               run_fn<typename T::list_aos>(_opts);
           } else {
               run_fn<typename T::list_soa>(_opts);
           }
           break;
       case opts::container_type::deque:
           if (_opts.ds == opts::data_structure::aos) {
               run_fn<typename T::deque_aos>(_opts);
           } else {
               run_fn<typename T::deque_soa>(_opts);
           }
           break;
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
            break;
        case 3:
            dispatch<type_map<type3>>(_opts);
            break;
        case 4:
            dispatch<type_map<type4>>(_opts);
            break;
    }
}

}
#endif

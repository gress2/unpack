#ifndef BENCHMARK_UTILS
#define BENCHMARK_UTILS

// Benchmarking unpacking strategies
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstring>
#include <deque>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "unpack.hpp"
#include "random_generator.hpp"
#include "convert_to_numeric.hpp"
#include "add_to_any.hpp"
#include "repeat_type_tuple.hpp"

namespace unpack_benchmark
{

template <typename It>
void fill_container_randomly(
  It&& begin,
  It&& end,
  std::size_t min = std::numeric_limits<std::size_t>::min(),
  std::size_t max = std::numeric_limits<std::size_t>::max(),
  std::size_t seed = std::random_device()()
) {
  random_generator rg;
  for (auto it = begin; it != end; ++it) {
    tuple_for_each([&rg](auto&& element) {
      rg.generate(element);
    }, *it);
  }
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

template <int N, class T>
constexpr T _pow(const T& x)
{
		return N > 1 ? x * _pow<(N - 1) * (N > 1)>(x)
         : N < 0 ? T(1) / _pow<(-N) * (N < 0)>(x)
             : N == 1 ? x
                  : T(1);
}

struct opts {
  enum data_structure { aos, soa };
  enum access_pattern { independent, single, combined };
  enum container_type { vector, list, deque };
  enum _function { simple, complex };

  data_structure ds;
  container_type ct;
  size_t container_size;
  size_t type_index;
  _function fn;
  access_pattern ap;
  size_t loop_iterations;
  bool column = true;
  double time_limit = -1;

  // example: ./Release/bin/unpack_chrono_benchmark soa vector 1000000 1 simple independent 100
  opts(char* argv[], int argc) {
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
    if (type_index > 40) {
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

    loop_iterations = std::stoi(argv[7]);

    if (strncmp(argv[8], "column", 3) == 0) {
      column = true;
    } else if (strncmp(argv[8], "nocolumn", 3) == 0) {
      column = false;
    } else {
      throw std::runtime_error("Invalid column choice provided");
    }

    if (argc > 9) {
      time_limit = std::stod(argv[9]);
    }
  }
};

// SIMPLE OPS - SINGLE / INDEPENDENT ELEMENT ACCESS

template <typename T>
auto simple_op(T& t) -> decltype((void)(t *= 2)) {
  using type = typename std::remove_reference<decltype(t)>::type;
  t *= std::is_floating_point<type>::value ? static_cast<type>(1.0000001) :
    -t * (t == std::numeric_limits<type>::max() - 1) + (t + 1) *
    (t != std::numeric_limits<type>::max() - 1);
}

template <typename T>
auto simple_op(T& t) -> decltype((void)(std::get<0>(t))) {
  tuple_for_each([](auto& tuple_elem) {
    simple_op(tuple_elem);
  }, t);
}

void simple_op(std::string& s) {
  s.insert(0, 1, 'x');
  s.pop_back();
}

template <typename T>
void simple_op_helper(T& t) {
  for (auto it = t.begin(); it != t.end(); ++it) {
    simple_op(*it);
  }
}

template <typename...T>
void simple_op(T&... t) {
  simple_op_helper(t...);
}


template <typename T>
void simple_single_raw(std::vector<unpack<T>>& container) {
  for (auto it = container.template begin<0>(); it != container.template end<0>(); ++it) {
    simple_op(*it);
  }
}

template <typename T>
void simple_single_raw(T& container) {
  for (auto&& element : container) {
    simple_op(std::get<0>(element));
  }
}

template <typename T>
void simple_single(T& container, size_t iterations, bool column) {
  for (size_t i = 0; i < iterations; i++) {
    if (column) {
      simple_single_raw(container);
    } else {
      for (auto&& element : container) {
        simple_op(std::get<0>(element));
      }
    }
  }
}

template <typename T>
void simple_independent(T& container, size_t iterations) {
  for (size_t i = 0; i < iterations; i++) {
    for (auto&& element : container) {
      tuple_for_each([](auto&& tuple_elem) {
         simple_op(tuple_elem);
      }, std::forward<decltype(element)>(element));
    }
  }
}

// COMPLEX OPS - SINGLE / INDEPENDENT ELEMENT ACCESS

template <typename T>
typename std::enable_if<std::is_integral<typename std::remove_reference<T>::type>
       ::value>::type complex_op(T& t) {
  t += _pow<50>(t) % 50;
}

template <typename T>
typename std::enable_if<std::is_floating_point<typename std::remove_reference<T>::type>
       ::value>::type complex_op(T& t) {
  t = cos(t);
}

template <typename T>
auto complex_op(T& t) -> decltype((void)(std::get<0>(t))) {
  tuple_for_each([](auto& tuple_elem) {
    complex_op(tuple_elem);
  }, t);
}

void complex_op(std::string& s) {
  std::hash<std::string> hash_fn;
  if (hash_fn(s) % 2 == 0) {
    s.insert(0, 1, 'x');
  } else {
    s.pop_back();
  }
}

template <typename T>
void complex_op_helper(T& t) {
  for (auto it = t.begin(); it != t.end(); ++it) {
    complex_op(*it);
  }
}

template <typename...T>
void complex_op(T&... t) {
   complex_op_helper(t...);
}

template <typename T>
void complex_single(T& container, size_t iterations) {
  for (size_t i = 0; i < iterations; i++) {
    for (auto&& element : container) {
      complex_op(std::get<0>(element));
    }
  }
}

template <typename T>
void complex_independent(T& container, size_t iterations) {
  for (size_t i = 0; i < iterations; i++) {
    for (auto&& element : container) {
      tuple_for_each([](auto&& tuple_elem) {
			   complex_op(std::forward
			     <decltype(tuple_elem)>(tuple_elem));
      }, std::forward<decltype(element)>(element));
    }
  }
}

// SIMPLE OPS - COMBINED ELEMENT ACCESS

double sum() {
  return 1;
}

template <typename T, typename... Tail>
auto sum(T&& t, Tail&&... tail) {
  return convert(t) + sum(std::forward<Tail>(tail)...);
}

template <typename T, size_t... Indices>
auto simple_combined_fn_impl(T&& t, std::index_sequence<Indices...>) {
  return sum(std::get<Indices>(t)...);
}

template <typename T>
void simple_combined_fn(T&& t) {
  constexpr auto size = std::tuple_size<typename std::remove_reference<T>::type>{};
  auto fn_res = simple_combined_fn_impl(std::forward<T>(t),
          std::make_index_sequence<size>{});
  tuple_for_each([&fn_res](auto& tuple_elem) {
    add_to_any(fn_res, tuple_elem);
  }, std::forward<T>(t));
}

template <typename T>
void simple_combined(T& container, size_t iterations) {
  for (size_t i = 0; i < iterations; i++) {
    for (auto&& element : container) {
      simple_combined_fn(std::forward<decltype(element)>(element));
    }
  }
}

// COMPLEX OPS - COMBINED ELEMENT ACCESS

double strange_addition() {
  return 0;
}

template <typename T, typename... Tail>
auto strange_addition(T&& t, Tail&&... tail) {
  return pow(convert(std::forward<T>(t)), 4 + cos(convert(std::forward<T>(t))))
    + strange_addition(std::forward<Tail>(tail)...);
}

template <typename T, size_t... Indices>
auto complex_combined_fn_impl(T&& t, std::index_sequence<Indices...>) {
  return cos(std::log(sin(strange_addition(std::get<Indices>(t)...))));
}

template <typename T>
void complex_combined_fn(T&& t) {
  constexpr auto size = std::tuple_size<typename std::remove_reference<T>::type>{};
  auto fn_res = complex_combined_fn_impl(std::forward<T>(t),
          std::make_index_sequence<size>{});
  tuple_for_each([&fn_res](auto&& tuple_elem) {
    add_to_any(fn_res, tuple_elem);
  }, std::forward<T>(t));
}

template <typename T>
void complex_combined(T& container, size_t iterations) {
  for (size_t i = 0; i < iterations; i++) {
    for (auto&& element : container) {
      complex_combined_fn(std::forward<decltype(element)>(element));
    }
  }
}

template <typename T, typename F>
unsigned char run_fn(opts& _opts, F& start_timing) {
  T container(_opts.container_size);
  fill_container_randomly(container.begin(), container.end());
  start_timing();
  if (_opts.fn == opts::_function::simple) {
    switch(_opts.ap) {
      case opts::access_pattern::single:
        simple_single(container, _opts.loop_iterations, _opts.column);
        break;
      case opts::access_pattern::independent:
        simple_independent(container, _opts.loop_iterations);
        break;
      case opts::access_pattern::combined:
        simple_combined(container, _opts.loop_iterations);
        break;
    }
  } else {
    switch(_opts.ap) {
      case opts::access_pattern::single:
        complex_single(container, _opts.loop_iterations);
        break;
      case opts::access_pattern::independent:
        complex_independent(container, _opts.loop_iterations);
        break;
      case opts::access_pattern::combined:
        complex_combined(container, _opts.loop_iterations);
        break;
    }
  }
  return check_bytes(container.begin(), container.end());
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

template <typename T, typename F>
unsigned char dispatch(opts& _opts, F& start_timing) {
   switch (_opts.ct) {
     case opts::container_type::vector:
       if (_opts.ds == opts::data_structure::aos) {
         return run_fn<typename T::vector_aos>(_opts, start_timing);
       } else {
         return run_fn<typename T::vector_soa>(_opts, start_timing);
       }
       break;
     case opts::container_type::list:
       if (_opts.ds == opts::data_structure::aos) {
         return run_fn<typename T::list_aos>(_opts, start_timing);
       } else {
         return run_fn<typename T::list_soa>(_opts, start_timing);
       }
       break;
     case opts::container_type::deque:
       if (_opts.ds == opts::data_structure::aos) {
         return run_fn<typename T::deque_aos>(_opts, start_timing);
       } else {
         return run_fn<typename T::deque_soa>(_opts, start_timing);
       }
       break;
     default:
       return 'x';
   }
}

template <typename F>
unsigned char run_benchmark(opts& _opts, F& start_timing) {
  using type0 = typename std::tuple<char>;
  using type1 = typename std::tuple<int>;
  using type2 = typename std::tuple<double, double>;
  using type3 = typename repeat_type_tuple<double, 8>::type;
  using type4 = typename std::tuple<char, int, std::string, double>;
  using type5 = typename std::tuple<std::vector<int>>;
  using type6 = typename repeat_type_tuple<int, 4>::type;
  using type7 = typename repeat_type_tuple<double, 4>::type;
  using type8 = typename repeat_type_tuple<std::string, 4>::type;
  using type9 = typename std::tuple<std::vector<int>, std::vector<double>>;
  using type10 = typename std::tuple<int,double,std::string,std::vector<int>>;

  switch (_opts.type_index) {
    case 0:
      return dispatch<type_map<type0>>(_opts, start_timing);
      break;
    case 1:
      return dispatch<type_map<type1>>(_opts, start_timing);
      break;
    case 2:
      return dispatch<type_map<type2>>(_opts, start_timing);
      break;
    case 3:
      return dispatch<type_map<type3>>(_opts, start_timing);
      break;
    case 4:
      return dispatch<type_map<type4>>(_opts, start_timing);
      break;
    case 5:
      return dispatch<type_map<type5>>(_opts, start_timing);
      break;
    case 6:
      return dispatch<type_map<type6>>(_opts, start_timing);
      break;
    case 7:
      return dispatch<type_map<type7>>(_opts, start_timing);
      break;
    case 8:
      return dispatch<type_map<type8>>(_opts, start_timing);
      break;
    case 9:
      return dispatch<type_map<type9>>(_opts, start_timing);
      break;
    case 10:
      return dispatch<type_map<type10>>(_opts, start_timing);
      break;
    default:
      return 'x';
  }
}

}
#endif

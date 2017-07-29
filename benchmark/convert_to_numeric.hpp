#ifndef CONVERT_TO_NUMERIC
#define CONVERT_TO_NUMERIC

#include <array>
#include <string>
#include <tuple>

auto convert(std::string& s) {
  return s.size();
}

template <typename T>
auto convert(T& t) -> decltype(t *= 1) {
  return t;
}

template <typename T>
auto convert(T& t) -> decltype((double)(std::get<0>(t))) {
  double sum = 0;
  tuple_for_each([&sum](auto& tuple_elem) {
    sum += convert(tuple_elem);
  }, t); 
  return sum;
}

template <typename T>
double convert_helper(T& t) {
  double sum = 0;
  for (auto it = t.begin(); it != t.end(); ++it) {
    sum += convert(*it);   
  }
  return sum;
}

template <typename...T>
double convert(T&... t) {
  return convert_helper(t...);
}

#endif

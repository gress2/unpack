#ifndef ADD_TO_ANY
#define ADD_TO_ANY

#include <tuple>

#include "unpack.hpp"

template <typename T>
void add_to_any(T&& t, std::string&s) {
  if (static_cast<int>(t) % 2 == 0) {
    s.insert(0, 1, 'x');
    s.pop_back();
  } else {
    s.insert(0, 1, 'y');
    s.pop_back();
  } 
} 

template <typename T, typename U>
auto add_to_any(T&& t, U& u) -> decltype((void)(u + t)) {
  u += std::forward<T>(t);
}

template <typename T, typename U>
auto add_to_any(T&& t, U& u) -> decltype((void)(std::get<0>(u))) {
  tuple_for_each([&t](auto& tuple_elem) {
    add_to_any(std::forward<T>(t), tuple_elem);
  }, u);
}

template <typename T, typename U>
void add_to_any_helper(T&& t, U& u) {
  for (auto it = u.begin(); it != u.end(); ++it) {
    add_to_any(std::forward<T>(t), *it);
  }
}

template <typename T, typename... U>
void add_to_any(T&& t, U&... u) {
  add_to_any_helper(std::forward<T>(t), u...);    
}

#endif

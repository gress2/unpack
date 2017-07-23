#ifndef RANDOM_GENERATOR
#define RANDOM_GENERATOR

#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <random>
#include <tuple>

#include "unpack.hpp"

// Xorshift random engine
struct xorshift_engine {
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

struct string_distribution {
    std::string operator()() noexcept {
        auto randchar = []() -> char {
            const char charset[] =
                "0123456789"
                "ACDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
            const std::size_t max_index = (sizeof(charset) - 1);
            return charset[rand() % max_index];
        };
        std::string str(15, 0);
        std::generate_n(str.begin(), 15, randchar);
        return str;
    }
};

struct random_generator {
    private:
		std::uniform_real_distribution<double> _real;
        std::uniform_int_distribution<std::size_t> _int;
        string_distribution _string;
        xorshift_engine _xe;
    public:
        random_generator() : _xe(xorshift_engine(std::random_device()())) {}
        template <typename T>
        void generate(T& t, typename std::enable_if<std::is_floating_point<T>::value>::type *q = nullptr) {
            t = _real(_xe);
        }
        template <typename T>
        void generate(T& t, typename std::enable_if<std::is_integral<T>::value>::type *q = nullptr) {
            t = _int(_xe);
        }
        template <typename T>
        void generate(T& t, typename std::enable_if<std::is_same<std::string, T>::value>::type *q = nullptr) {
            t = _string();
        }
        template <typename T>
        auto generate(T& t) -> decltype((void)(std::get<0>(t))) {
            tuple_for_each([this](auto& tuple_element) {
                this->generate(tuple_element);
            }, t); 
        }
        template <typename T>
        auto gen_helper(T& t) -> decltype(t.resize(3)) {
            if (t.size() == 0) {
              t.resize(3);
            }
            for (auto it = t.begin(); it != t.end(); ++it) {
                generate(*it);
            }
        }

        template <typename T>
        void gen_helper_helper(T& t) {
            for (auto it = t.begin(); it != t.end(); ++it) {
                generate(*it);  
            }
        }

        template <typename... T>
        void gen_helper(T&... t) {
            gen_helper_helper(t...);
        }

        template <typename...T>
        void generate(T&... t) {
            gen_helper(t...);
        }
};

#endif

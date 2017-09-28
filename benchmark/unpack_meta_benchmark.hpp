// ========================= UNPACK META BENCHMARK ========================== //
// Project:         unpack
// Name:            unpack_meta_benchmark.hpp
// Description:     Runs benchmarks on unpack
// Contributor(s):  Vincent Reverdy [2017]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _UNPACK_META_BENCHMARK_HPP_INCLUDED
#define _UNPACK_META_BENCHMARK_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <list>
#include <array>
#include <cmath>
#include <ctime>
#include <deque>
#include <regex>
#include <tuple>
#include <chrono>
#include <cstdio>
#include <limits>
#include <memory>
#include <random>
#include <string>
#include <vector>
#include <cassert>
#include <cstring>
#include <fstream>
#include <numeric>
#include <sstream>
#include <utility>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <type_traits>
#include <forward_list>
// Project sources
// Third-party libraries
// Miscellaneous
// ========================================================================== //



/* ************************* TYPE TRAITS UTILITIES ************************** */
// Raw structure definition
template <class T>
struct raw
: std::remove_cv<typename std::remove_reference<T>::type>
{
};

// Raw type alias
template <class T>
using raw_t = typename raw<T>::type;

// To void structure definition
template <class... Args>
struct to_void
{
    using type = void;
};

// Void type alias
template <class... Args>
using void_t = typename to_void<Args...>::type;

// If type alias
template <template <class...> class Trait, class... Types>
using if_t = typename std::enable_if<Trait<Types...>::value>::type;

// If not type alias
template <template <class...> class Trait, class... Types>
using if_not_t = typename std::enable_if<!Trait<Types...>::value>::type;

// If invokable on type alias
template <class F, class... Args>
using if_invokable_t = decltype(std::declval<F>()(std::declval<Args>()...));

// Iterator category type alias
template <class T>
using iterator_category = typename std::iterator_traits<T>::iterator_category;
/* ************************************************************************** */



/* ****************************** TYPE TRAITS ******************************* */
// Is not a tuple structure definition
template <class T, class = void>
struct is_tuple
: std::false_type
{
};

// Is a tuple structure definition
template <class T>
struct is_tuple<T, void_t<decltype(std::tuple_size<raw_t<T>>::value)>>
: std::true_type
{
};

// Is not resizeable structure definition
template <class T, class = void>
struct is_resizeable
: std::false_type
{
};

// Is resizeable structure definition
template <class T>
struct is_resizeable<T, void_t<decltype(std::declval<raw_t<T>>().resize(0))>>
: std::true_type
{
};

// Is not a range structure definition
template <class T, class = void>
struct is_range
: std::false_type
{
};

// Is a range structure definition
template <class T>
struct is_range<T, void_t<decltype(std::begin(std::declval<raw_t<T>>()))>>
: std::true_type
{
};

// Is not a string structure definition
template <class T, class = void>
struct is_string
: std::false_type
{
};

// Is a string structure definition
template <class T>
struct is_string<T, void_t<decltype(std::declval<raw_t<T>>().c_str())>>
: std::true_type
{
};

// Is not an iterator structure definition
template <class T, class = void>
struct is_iterator
: std::false_type
{
};

// Is an iterator structure definition
template <class T>
struct is_iterator<T, void_t<iterator_category<raw_t<T>>>>
: std::true_type
{
};

// Is not column accessible structure definition
template <class T, class = void>
struct is_column_accessible
: std::false_type
{
};

// Is column accessible structure definition
template <class T>
struct is_column_accessible<T, void_t<decltype(raw_t<T>().template begin<0>())>>
: std::true_type
{
};

// Is container structure definition
template <class T, class = void>
struct is_container
: std::integral_constant<bool, is_range<T>::value && !is_string<T>::value>
{
};

// Is terminal structure definition
template <class T, class = void>
struct is_terminal
: std::integral_constant<bool, !is_container<T>::value && !is_tuple<T>::value>
{
};

// Is integral structure definition
template <class T, class = void>
struct is_integral
: std::integral_constant<bool, std::is_integral<raw_t<T>>::value>
{
};

// Is floating point structure definition
template <class T, class = void>
struct is_floating_point
: std::integral_constant<bool, std::is_floating_point<raw_t<T>>::value>
{
};
/* ************************************************************************** */



/* ******************************* TYPE NAME ******************************** */
// Type name structure declaration
template <class>
struct type_name;

// Type name structure definition: void specialization
template <>
struct type_name<void>
{
    static constexpr auto value = "void";
};

// Type name structure definition: std::nullptr_t specialization
template <>
struct type_name<std::nullptr_t>
{
    static constexpr auto value = "std::nullptr_t";
};

// Type name structure definition: bool specialization
template <>
struct type_name<bool>
{
    static constexpr auto value = "bool";
};

// Type name structure definition: char specialization
template <>
struct type_name<char>
{
    static constexpr auto value = "char";
};

// Type name structure definition: wchar_t specialization
template <>
struct type_name<wchar_t>
{
    static constexpr auto value = "wchar_t";
};

// Type name structure definition: signed char specialization
template <>
struct type_name<signed char>
{
    static constexpr auto value = "signed char";
};

// Type name structure definition: unsigned char specialization
template <>
struct type_name<unsigned char>
{
    static constexpr auto value = "unsigned char";
};

// Type name structure definition: short int specialization
template <>
struct type_name<short int>
{
    static constexpr auto value = "short int";
};

// Type name structure definition: unsigned short int specialization
template <>
struct type_name<unsigned short int>
{
    static constexpr auto value = "unsigned short int";
};

// Type name structure definition: int specialization
template <>
struct type_name<int>
{
    static constexpr auto value = "int";
};

// Type name structure definition: unsigned int specialization
template <>
struct type_name<unsigned int>
{
    static constexpr auto value = "unsigned int";
};

// Type name structure definition: long int specialization
template <>
struct type_name<long int>
{
    static constexpr auto value = "long int";
};

// Type name structure definition: unsigned long int specialization
template <>
struct type_name<unsigned long int>
{
    static constexpr auto value = "unsigned long int";
};

// Type name structure definition: long long int specialization
template <>
struct type_name<long long int>
{
    static constexpr auto value = "long long int";
};

// Type name structure definition: unsigned long long int specialization
template <>
struct type_name<unsigned long long int>
{
    static constexpr auto value = "unsigned long long int";
};

// Type name structure definition: float specialization
template <>
struct type_name<float>
{
    static constexpr auto value = "float";
};

// Type name structure definition: double specialization
template <>
struct type_name<double>
{
    static constexpr auto value = "double";
};

// Type name structure definition: long double specialization
template <>
struct type_name<long double>
{
    static constexpr auto value = "long double";
};

// Type name structure definition: std::tuple specialization
template <class... T>
struct type_name<std::tuple<T...>>
{
    static constexpr auto value = "std::tuple";
};

// Type name structure definition: std::pair specialization
template <class T1, class T2>
struct type_name<std::pair<T1, T2>>
{
    static constexpr auto value = "std::pair";
};

// Type name structure definition: std::array specialization
template <class T, std::size_t N>
struct type_name<std::array<T, N>>
{
    static constexpr auto value = "std::array";
};

// Type name structure definition: std::vector specialization
template <class... T>
struct type_name<std::vector<T...>>
{
    static constexpr auto value = "std::vector";
};

// Type name structure definition: std::deque specialization
template <class... T>
struct type_name<std::deque<T...>>
{
    static constexpr auto value = "std::deque";
};

// Type name structure definition: std::list specialization
template <class... T>
struct type_name<std::list<T...>>
{
    static constexpr auto value = "std::list";
};

// Type name structure definition: std::forward_list specialization
template <class... T>
struct type_name<std::forward_list<T...>>
{
    static constexpr auto value = "std::forward_list";
};

// Type name structure definition: std::string specialization
template <>
struct type_name<std::string>
{
    static constexpr auto value = "std::string";
};

// Type name structure definition: std::basic_string specialization
template <class... T>
struct type_name<std::basic_string<T...>>
{
    static constexpr auto value = "std::basic_string";
};

// Type name structure definition: std::allocator specialization
template <class T>
struct type_name<std::allocator<T>>
{
    static constexpr auto value = "std::allocator";
};

// Type name structure definition: std::char_traits specialization
template <class T>
struct type_name<std::char_traits<T>>
{
    static constexpr auto value = "std::char_traits";
};
/* ************************************************************************** */



/* ****************************** TYPE STRING ******************************* */
// Type string structure declaration
template <class...>
struct type_string;

// Type string structure definition: empty specialization
template <>
struct type_string<>
{
    static std::string name() {
        return {};
    }
};

// Type string structure definition: single type specialization
template <class T>
struct type_string<T>
{
    static std::string name() {
        return type_name<T>::value;
    }
};

// Type string structure definition: std::string specialization
template <>
struct type_string<std::string>
{
    static std::string name() {
        return type_name<std::string>::value;
    }
};

// Type string structure definition: multiple types specialization
template <class T0, class... TN>
struct type_string<T0, TN...>
{
    static std::string name() {
        return type_string<T0>::name() + ", " + type_string<TN...>::name();
    }
};

// Type string structure definition: class template specialization
template <template <class...> class C, class... T>
struct type_string<C<T...>>
{
    static std::string name() {
        using type = C<T...>;
        static const std::string value = type_name<type>::value;
        return value + "<" + type_string<T...>::name() + ">";
    }
};

// Type string structure definition: container specialization
template <template <class, class> class C, class T>
struct type_string<C<T, std::allocator<T>>>
{
    static std::string name() {
        using type = C<T, std::allocator<T>>;
        static const std::string value = type_name<type>::value;
        return value + "<" + type_string<T>::name() + ">";
    }
};

// Type string structure definition: generic string specialization
template <template <class, class, class> class C, class T>
struct type_string<C<T, std::char_traits<T>, std::allocator<T>>>
{
    static std::string name() {
        using type = C<T, std::char_traits<T>, std::allocator<T>>;
        static const std::string value = type_name<type>::value;
        return value + "<" + type_string<T>::name() + ">";
    }
};

// Type string structure definition: array specialization
template <template <class, std::size_t> class C, class T, std::size_t N>
struct type_string<C<T, N>>
{
    static std::string name() {
        using type = C<T, N>;
        static const std::string size = std::to_string(N);
        static const std::string value = type_name<type>::value;
        return value + "<" + type_string<T>::name() + ", " + size + ">";
    }
};
/* ************************************************************************** */



/* ***************************** TYPE SELECTOR ****************************** */
// Type selector structure definition
template <class... T>
struct type_selector
{
    // Types
    template <std::size_t I>
    using enable_if = typename std::enable_if<(I < sizeof...(T))>::type;
    template <class Dummy>
    using string_type = std::string;
    
    // Lifecycle
    explicit type_selector(): types(), names(type_string<T>::name()...) {
    };

    // Execution of the provided function on the type with the provided name
    template <std::size_t I = 0, class F, class = enable_if<I>>
    auto operator()(F&& f, const std::string& name) const {
        static constexpr std::size_t next = I + (I < sizeof...(T));
        return compare(name, std::get<I>(names))
            ? std::forward<F>(f)(std::get<I>(types))
            : this->operator()<next>(std::forward<F>(f), name);
    }
    template <std::size_t I = 0, class F, class... Dummy>
    auto operator()(F&&, const std::string& name, Dummy...) const {
        static const std::string what = "type_selector::operator()";
        throw std::invalid_argument(what + "(f, " + name + ")");
    }

    // Conversion of the names to a string
    template <std::size_t I = 0, class = enable_if<I>>
    std::string to_string() {
        static constexpr std::size_t next = I + (I < sizeof...(T));
        static constexpr unsigned int i = I;
        static constexpr int d = digits10(sizeof...(T));
        static constexpr auto fmt = "[%0*d] %s\n";
        const std::size_t n = std::strlen(fmt) + d + std::get<I>(names).size();
        std::vector<char> buf(n, '\0');
        std::snprintf(buf.data(), n, fmt, d, i, std::get<I>(names).c_str());
        return std::string(std::begin(buf), std::end(buf)) + to_string<next>();
    }
    template <std::size_t I = 0, class... Dummy>
    std::string to_string(Dummy...) {
        return {};
    }

    // Comparison of strings
    static bool compare(std::string lhs, std::string rhs) {
        auto replacer = [](std::string& str){
            const std::string from = "std::";
            const std::string to = "";
            std::size_t position = 0;
            while((position = str.find(from, position)) != str.npos) {
                str.replace(position, from.size(), to);
                position += to.size();
            }
        };
        auto remover = [](std::string& str){
            return std::remove_if(std::begin(str), std::end(str), [](int c){
                return std::isspace(c);
            });
        };
        replacer(lhs);
        replacer(rhs);
        lhs.erase(remover(lhs), std::end(lhs));
        rhs.erase(remover(rhs), std::end(rhs));
        return lhs == rhs;
    }

    // Compute the number of digits in base 10 of the provided number
    static constexpr std::size_t digits10(std::size_t n) {
        std::size_t i = 0;
        for (i = 1; n >= 10; ++i, n /= 10) {
        }
        return i;
    }

    // Data members
    const std::tuple<T...> types;
    const std::tuple<string_type<T>...> names;
};
/* ************************************************************************** */



/* ******************************** TO TUPLE ******************************** */
// To tuple structure declaration
template <class...>
struct to_tuple;

// To tuple structure definition: recursion
template <class T, std::size_t... I>
struct to_tuple<T, std::index_sequence<I...>>
{
    using type = std::tuple<typename std::tuple_element<I, T>::type...>;
};

// To tuple structure definition: create a tuple from another product type
template <class T>
struct to_tuple<T>
: to_tuple<T, std::make_index_sequence<std::tuple_size<T>::value>>
{
};

// To tuple type alias
template <class T>
using to_tuple_t = typename to_tuple<T>::type;

// Ntuple type alias
template <class T, std::size_t N>
using ntuple = to_tuple_t<std::array<T, N>>;
/* ************************************************************************** */



/* **************************** XORSHIFT ENGINE ***************************** */
// Xorshift engine structure definition
struct xorshift_engine
{
    // Types
    using type = unsigned int;
    using result_type = type;

    // Lifecycle
    explicit constexpr xorshift_engine(result_type seed = 0) noexcept
    : state(seed ? seed : !seed) {
    }

    // Minimum
    static constexpr result_type min() noexcept {
        return std::numeric_limits<result_type>::min();
    }

    // Maximum
    static constexpr result_type max() noexcept {
        return std::numeric_limits<result_type>::max();
    }

    // Generation
    constexpr result_type operator()() noexcept {
        constexpr std::size_t shift0 = 13;
        constexpr std::size_t shift1 = 17;
        constexpr std::size_t shift2 = 5;
        state ^= state << shift0;
        state ^= state >> shift1;
        state ^= state << shift2;
        return state;
    }

    // Data member
    type state;
};
/* ************************************************************************** */



/* **************************** RANDOM GENERATOR **************************** */
template <class Engine = xorshift_engine, std::size_t StringSize = 16>
struct random_generator
{
    // Types and constants
    using engine_type = Engine;
    static constexpr std::size_t string_size = StringSize;
    static constexpr std::size_t npos = -1;
    static constexpr std::size_t imin = 1;
    static constexpr std::size_t imax = std::numeric_limits<std::size_t>::max();
    static constexpr double rmin = 0 + std::numeric_limits<double>::epsilon();
    static constexpr double rmax = 1 - std::numeric_limits<double>::epsilon();
    static constexpr char cmin = '0';
    static constexpr char cmax = '9';
    
    // Lifecycle
    explicit random_generator(std::size_t seed = npos)
    : engine(), idist(imin, imax), rdist(rmin, rmax), cdist(cmin, cmax) {
        std::random_device device;
        engine = engine_type(seed == npos ? device() : seed);
    }
    
    template <class T, class = if_t<is_integral, T>>
    T get() {
        return idist(engine) % std::numeric_limits<T>::max();
    }
    template <class T, class = if_t<is_floating_point, T>>
    T get(std::nullptr_t = nullptr) {
        return rdist(engine);
    }
    template <class T, class = if_t<is_string, T>>
    T get(decltype(std::ignore) = std::ignore) {
        std::string string(string_size, '\0');
        for (auto& c: string) {
            c = cdist(engine);
        }
        return string;
    }

    // Data members
    engine_type engine;
    std::uniform_int_distribution<std::size_t> idist;
    std::uniform_real_distribution<double> rdist;
    std::uniform_int_distribution<char> cdist;
};
/* ************************************************************************** */



/* *************************** RANDOM INITIALIZER *************************** */
// Random initializer structure definition
template <class Generator = random_generator<>, std::size_t ContainerSize = 3>
struct random_initializer
{
    // Types and constants
    using generator_type = Generator;
    static constexpr std::size_t container_size = ContainerSize;
    template <class T, class G = generator_type>
    using if_generatable_t = decltype(std::declval<G>().template get<T>());
    
    // Lifecycle
    explicit random_initializer(generator_type g = Generator()): generator(g) {
    }

    // Initialization
    template <class It, class = if_t<is_iterator, It>>
    void operator()(It first, It last) {
        for (auto it = first; it != last; ++it) {
            this->operator()(*it);
        }
    }
    template <class C, class... Dummy, class = if_t<is_resizeable, C>>
    void operator()(C&& container, Dummy...) {
        if (container_size != 0 && container.size() == 0) {
            container.resize(container_size);
        }
        this->operator()(std::begin(container), std::end(container));
    }
    template <class T, std::size_t... I>
    void operator()(T&& tuple, std::index_sequence<I...>) {
        using dummy = int[];
        (void)dummy{1, (
            this->operator()(std::get<I>(std::forward<T>(tuple))), void(), int{}
        )...};
    }
    template <class T, std::size_t N = std::tuple_size<raw_t<T>>::value>
    void operator()(T&& tuple, decltype(std::ignore) = std::ignore) {
        this->operator()(std::forward<T>(tuple), std::make_index_sequence<N>());
    }
    template <class T, class = if_generatable_t<raw_t<T>>>
    void operator()(T&& value) {
        std::forward<T>(value) = generator.template get<raw_t<T>>();
    }

    // Data members
    generator_type generator;
};
/* ************************************************************************** */



/* ****************************** BYTE CHECKER ****************************** */
// Byte checker structure definition
struct byte_checker
{
    // Types and constants
    using byte_type = unsigned char;
    static constexpr byte_type zero = 0;

    // Lifecycle
    explicit byte_checker(): state(!std::time(nullptr)) {
    }
    template <class T>
    explicit byte_checker(const T& x): state(!std::time(nullptr)) {
        this->operator()(x);
    }

    // Get value while preventing optimizations
    bool get() {
        static thread_local volatile byte_type tmp = zero;
        std::string what = std::to_string(state);
        state += *reinterpret_cast<const volatile byte_type*>(&tmp);
        try {
            if (!state) {
                throw std::runtime_error(what);
            }
        } catch (const std::runtime_error& e) {
            if (state) {
                std::cerr << "invoker::operator()" << std::endl;
            }
        }
        return state;
    }

    // Computation
    template <class It, class = if_t<is_iterator, It>>
    byte_type operator()(It first, It last) {
        byte_type byte = zero;
        for (auto it = first; it != last; ++it) {
            byte += this->operator()(*it);
        }
        state += byte;
        return byte;
    }
    template <class Range, class = if_t<is_range, Range>>
    byte_type operator()(const Range& range) {
        return this->operator()(std::begin(range), std::end(range));
    }
    template <class Type, class = if_not_t<is_range, Type>>
    byte_type operator()(const Type& value, std::nullptr_t = nullptr) {
        const byte_type* first = reinterpret_cast<const byte_type*>(&value);
        const byte_type* last = first + sizeof(Type);
        const byte_type byte = std::accumulate(first, last, zero);
        state += byte;
        return byte;
    }

    // Data members
    volatile byte_type state;
};
/* ************************************************************************** */



/* ******************************** OPTIONS ********************************* */
// Option structure definition
struct option
{
    enum struct tool {system, chrono, google, googles};
    enum struct orientation {aos, soa};
    enum struct container {vector};
    enum struct type {};
    enum struct complexity {simple, complex, branching};
    enum struct access {single, independent, combined, column};
};

// Option type name structure declaration
template <class>
struct option_type_name;

// Option type name structure definition: tool specialization
template <>
struct option_type_name<option::tool>
{
    static constexpr auto value = "tool";
};

// Option type name structure definition: orientation specialization
template <>
struct option_type_name<option::orientation>
{
    static constexpr auto value = "orientation";
};

// Option type name structure definition: container specialization
template <>
struct option_type_name<option::container>
{
    static constexpr auto value = "container";
};

// Option type name structure definition: type specialization
template <>
struct option_type_name<option::type>
{
    static constexpr auto value = "type";
};

// Option type name structure definition: complexity specialization
template <>
struct option_type_name<option::complexity>
{
    static constexpr auto value = "complexity";
};

// Option type name structure definition: access specialization
template <>
struct option_type_name<option::access>
{
    static constexpr auto value = "access";
};

// Option name structure declaration
template <class Type, Type Value>
struct option_name;

// Option name structure definition: system specialization
template <>
struct option_name<option::tool, option::tool::system>
{
    static constexpr auto value = "system";
};

// Option name structure definition: chrono specialization
template <>
struct option_name<option::tool, option::tool::chrono>
{
    static constexpr auto value = "chrono";
};

// Option name structure definition: google specialization
template <>
struct option_name<option::tool, option::tool::google>
{
    static constexpr auto value = "google";
};

// Option name structure definition: googles specialization
template <>
struct option_name<option::tool, option::tool::googles>
{
    static constexpr auto value = "googles";
};

// Option name structure definition: aos specialization
template <>
struct option_name<option::orientation, option::orientation::aos>
{
    static constexpr auto value = "aos";
};

// Option name structure definition: soa specialization
template <>
struct option_name<option::orientation, option::orientation::soa>
{
    static constexpr auto value = "soa";
};

// Option name structure definition: vector specialization
template <>
struct option_name<option::container, option::container::vector>
{
    static constexpr auto value = "vector";
};

// Option name structure definition: simple specialization
template <>
struct option_name<option::complexity, option::complexity::simple>
{
    static constexpr auto value = "simple";
};

// Option name structure definition: complex specialization
template <>
struct option_name<option::complexity, option::complexity::complex>
{
    static constexpr auto value = "complex";
};

// Option name structure definition: branching specialization
template <>
struct option_name<option::complexity, option::complexity::branching>
{
    static constexpr auto value = "branching";
};

// Option name structure definition: single specialization
template <>
struct option_name<option::access, option::access::single>
{
    static constexpr auto value = "single";
};

// Option name structure definition: independent specialization
template <>
struct option_name<option::access, option::access::independent>
{
    static constexpr auto value = "independent";
};

// Option name structure definition: combined specialization
template <>
struct option_name<option::access, option::access::combined>
{
    static constexpr auto value = "combined";
};

// Option name structure definition: column specialization
template <>
struct option_name<option::access, option::access::column>
{
    static constexpr auto value = "column";
};

// Option type structure definition
template <class Type, Type Value>
struct option_type
{
    static constexpr Type value = Value;
};

// Type string structure definition: option type specialization
template <class Type, Type Value>
struct type_string<option_type<Type, Value>>
{
    static std::string name() {
        return option_name<Type, Value>::value;
    }
};

// Option selector
template <class Type, Type... Value>
using option_selector = type_selector<option_type<Type, Value>...>;

// Options structure definition
template <
    option::tool Tool,
    option::orientation Orientation,
    option::container Container,
    class Type,
    option::complexity Complexity,
    option::access Access
>
struct options
{
    static constexpr option::tool tool = Tool;
    static constexpr option::orientation orientation = Orientation;
    static constexpr option::container container = Container;
    using type = Type;
    static constexpr option::complexity complexity = Complexity;
    static constexpr option::access access = Access;
};
/* ************************************************************************** */



/* *********************************  MATH ********************************** */
struct math
{
    // Signum
    template <class T>
    static constexpr T signum(T x) {
        constexpr T zero = T();
        return (zero < x) - (x < zero);
    } 

    // Power
    template <int N, class T>
    static constexpr T metapow(const T& x) {
        return N > 1 ? x * metapow<(N - 1) * (N > 1)>(x)
            : N < 0 ? static_cast<T>(1) / metapow<-N * (N < 0)>(x)
                : N == 1 ? x
                    : static_cast<T>(1);
    }

    // Modular exponentiation
    template <unsigned char E, unsigned char M, class T, std::size_t N = M>
    static constexpr T metapowm(const T& b, const T& c = static_cast<T>(1)) {
        constexpr unsigned char next = (E - 1) * (E > 0);
        constexpr std::size_t max = std::numeric_limits<T>::max();
        using check = typename std::enable_if<(N * N < max), T>::type;
        return M > 1 ? E > 0 ? metapowm<next, M, check>(b, c * b % T(M)) : c : 0;
    }
    
    // Square root
    template <class T>
    static constexpr T sqrt(T x, T current, T previous = T()) {
        constexpr T half = static_cast<T>(1) / static_cast<T>(2);
        const bool ok = !(current < previous) && !(current > previous);
        return ok ? current : sqrt(x, half * (current + x / current), current);
    }
    template <class T>
    static constexpr T sqrt(T x) {
        constexpr bool is_floating_point = std::is_floating_point<T>::value;
        return sqrt<typename std::enable_if<is_floating_point, T>::type>(x, x);
    }
};
/* ************************************************************************** */



/* ************************ PARAMETERIZED CONTAINER ************************* */
// Parameterized container structure declaration
template <
    option::orientation,
    option::container,
    class,
    template <class...> class
>
struct parameterized_container;

// Parameterized container structure definition: aos vector specialization
template <class ValueType, template <class...> class Unpacker>
struct parameterized_container<
    option::orientation::aos,
    option::container::vector,
    ValueType,
    Unpacker
>
{
    using type = std::vector<ValueType>;
};

// Parameterized container structure definition: soa vector specialization
template <class ValueType, template <class...> class Unpacker>
struct parameterized_container<
    option::orientation::soa,
    option::container::vector,
    ValueType,
    Unpacker
>
{
    using type = std::vector<Unpacker<ValueType>>;
};
/* ************************************************************************** */



/* ******************************* OPERATIONS ******************************* */
// Operation structure declaration
template <option::complexity>
struct operation;

// Operation structure definition: simple operation specialization
template <>
struct operation<option::complexity::simple>
{
    // Types
    using type = double;
    
    // Apply
    template <class T, class... X, class = if_t<is_integral, T>>
    constexpr type operator()(T& x, X...) {
        constexpr T zero = 0;
        constexpr T max = std::numeric_limits<T>::max();
        x = -(x - math::signum(x)) + (x == zero) * max;
        assert(x >= std::numeric_limits<T>::min() && x <= max);
        return x;
    }
    template <class T, class... X, class = if_t<is_floating_point, T>>
    constexpr type operator()(T& x, X&...) {
        constexpr T one = 1;
        constexpr T eps = std::numeric_limits<T>::epsilon();
        x = (one + std::fabs(x)) / (std::fabs(x) + eps);
        assert(std::isnormal(x));
        return x;
    }
    template <class T, class... Dummy, class = if_t<is_string, T>>
    constexpr type operator()(T& x) {
        const std::size_t size = x.size();
        x.insert(0, 1, x.back());
        x.resize(size);
        assert(size > 0 && x.size() == size);
        return size;
    }
};

// Operation structure definition: complex operation specialization
template <>
struct operation<option::complexity::complex>
{
    // Types
    using type = double;
    
    // Apply
    template <class T, class... X, class = if_t<is_integral, T>>
    constexpr type operator()(T& x, X...) {
        constexpr T zero = 0;
        constexpr T exp = 3;
        constexpr T mod = 11;
        constexpr T max = std::numeric_limits<T>::max();
        x = -(x - math::signum(x) * mod) + (x == zero) * max;
        x = math::metapowm<exp, mod>(x);
        assert(x >= std::numeric_limits<T>::min() && x <= max);
        return x;
    }
    template <class T, class... X, class = if_t<is_floating_point, T>>
    constexpr type operator()(T& x, X&...) {
        constexpr T one = 1;
        constexpr T eps = std::numeric_limits<T>::epsilon();
        x = std::tanh((one + std::fabs(x)) / (std::fabs(x) + eps));
        assert(std::isnormal(x));
        return x;
    }
    template <class T, class = if_t<is_string, T>>
    constexpr type operator()(T& x) {
        constexpr auto limit = std::numeric_limits<unsigned int>::digits10 + 1;
        const std::size_t size = x.size();
        std::size_t count = 0;
        char buffer[limit] = {};
        std::snprintf(buffer, limit, "%u", static_cast<unsigned int>(size));
        count = std::strlen(buffer);
        x.insert(0, buffer);
        std::rotate(std::begin(x), std::begin(x) + count, std::end(x));
        x.resize(size);
        std::rotate(std::begin(x), std::begin(x) + 1, std::end(x));
        assert(size > 0 && x.size() == size);
        return size;
    }
};

// Operation structure definition: branching operation specialization
template <>
struct operation<option::complexity::branching>
{
    // Types
    using type = double;
    
    // Apply
    template <class T, class... X, class = if_t<is_integral, T>>
    constexpr type operator()(T& x, X...) {
        constexpr T zero = 0;
        constexpr T one = 1;
        constexpr T max = std::numeric_limits<T>::max();
        if (x > zero) {
            x = one - x;
        } else if (x < zero) {
            x = -x - one;
        } else {
            x += max;
        }
        assert(x >= std::numeric_limits<T>::min() && x <= max);
        return x;
    }
    template <class T, class... X, class = if_t<is_floating_point, T>>
    constexpr type operator()(T& x, X&...) {
        constexpr T zero = 0;
        constexpr T one = 1;
        constexpr T eps = std::numeric_limits<T>::epsilon();
        if (x > zero) {
            if (x > eps) {
                x = (one + x) / x;
            } else {
                x = (one + eps) / eps;
            }
        } else {
            if (x < -eps) {
                x = (x - one) / x;
            } else {
                x = (-eps - one) / -eps;
            }
        }
        assert(std::isnormal(x));
        return x;
    }
    template <class T, class... Dummy, class = if_t<is_string, T>>
    constexpr type operator()(T& x) {
        const std::size_t size = x.size();
        if (x.front() & 1) {
            x.insert(0, 1, x.back());
        } else {
            x.insert(0, 1, x.front());
            std::swap(x.front(), x[size]);
        }
        x.resize(size);
        assert(size > 0 && x.size() == size);
        return size;
    }
};
/* ************************************************************************** */



/* ******************************** INVOKER ********************************* */
// Invoker structure declaration
template <option::access>
struct invoker;

// Invoker structure declaration: single specialization
template <>
struct invoker<option::access::single>
{
    // Call
    template <class F, class T, std::size_t N = std::tuple_size<raw_t<T>>()()>
    constexpr void operator()(F&& f, T&& x) {
        invoke(std::forward<F>(f), std::get<N / 2>(std::forward<T>(x)));
    }

    // Invocation
    template <class F, class T, class... D, class = if_t<is_container, T>>
    static constexpr void invoke(F&& f, T&& x, D...) {
        for (auto&& e: std::forward<T>(x)) {
            invoke(std::forward<F>(f), std::forward<decltype(e)>(e));
        } 
    }
    template <class F, class T, std::size_t... I, class = if_t<is_tuple, T>>
    static constexpr void invoke(F&& f, T&& x, std::index_sequence<I...>) {
        using dummy = int[];
        (void)dummy{1, (
            invoke(std::forward<F>(f), std::get<I>(std::forward<T>(x))),
        void(), int{})...};
    }
    template <class F, class T, class = if_t<is_tuple, T>>
    static constexpr void invoke(F&& f, T&& x, int = int{}) {
        using s = std::make_index_sequence<std::tuple_size<raw_t<T>>::value>;
        invoke(std::forward<F>(f), std::forward<T>(x), s());
    }
    template <class F, class T, class = if_t<is_terminal, T>>
    static constexpr void invoke(F&& f, T&& x) {
        std::forward<F>(f)(std::forward<T>(x));
    }
};

// Invoker structure declaration: independent specialization
template <>
struct invoker<option::access::independent>
{
    // Call
    template <class F, class T, std::size_t = std::tuple_size<raw_t<T>>()()>
    constexpr void operator()(F&& f, T&& x) {
        invoke(std::forward<F>(f), std::forward<T>(x));
    }

    // Invocation
    template <class F, class T, class... D, class = if_t<is_container, T>>
    static constexpr void invoke(F&& f, T&& x, D...) {
        for (auto&& e: std::forward<T>(x)) {
            invoke(std::forward<F>(f), std::forward<decltype(e)>(e));
        } 
    }
    template <class F, class T, std::size_t... I, class = if_t<is_tuple, T>>
    static constexpr void invoke(F&& f, T&& x, std::index_sequence<I...>) {
        using dummy = int[];
        (void)dummy{1, (
            invoke(std::forward<F>(f), std::get<I>(std::forward<T>(x))),
        void(), int{})...};
    }
    template <class F, class T, class = if_t<is_tuple, T>>
    static constexpr void invoke(F&& f, T&& x, int = int{}) {
        using s = std::make_index_sequence<std::tuple_size<raw_t<T>>::value>;
        invoke(std::forward<F>(f), std::forward<T>(x), s());
    }
    template <class F, class T, class = if_t<is_terminal, T>>
    static constexpr void invoke(F&& f, T&& x) {
        std::forward<F>(f)(std::forward<T>(x));
    }
};

// Invoker structure declaration: combined specialization
template <>
struct invoker<option::access::combined>
{
    // Call
    template <class F, class T, std::size_t = std::tuple_size<raw_t<T>>()()>
    void operator()(F&& f, T&& x) {
        using type = typename raw_t<F>::type;
        volatile type sum = type();
        auto lambda = [&sum, &f](auto&& x){
            constexpr type one = 1;
            constexpr type half = one / (one + one);
            type value = std::forward<F>(f)(std::forward<decltype(x)>(x));
            sum += ((one + std::fabs(value)) / (std::fabs(value) + half)) - one;
        };
        invoke(lambda, std::forward<T>(x));
        assert(std::isfinite(sum));
        if (!std::isfinite(sum)) {
            std::cerr << "invoker::operator()" << std::endl;
        }
    }

    // Invocation
    template <class F, class T, class... D, class = if_t<is_container, T>>
    static constexpr void invoke(F&& f, T&& x, D...) {
        for (auto&& e: std::forward<T>(x)) {
            invoke(std::forward<F>(f), std::forward<decltype(e)>(e));
        } 
    }
    template <class F, class T, std::size_t... I, class = if_t<is_tuple, T>>
    static constexpr void invoke(F&& f, T&& x, std::index_sequence<I...>) {
        using dummy = int[];
        (void)dummy{1, (
            invoke(std::forward<F>(f), std::get<I>(std::forward<T>(x))),
        void(), int{})...};
    }
    template <class F, class T, class = if_t<is_tuple, T>>
    static constexpr void invoke(F&& f, T&& x, int = int{}) {
        using s = std::make_index_sequence<std::tuple_size<raw_t<T>>::value>;
        invoke(std::forward<F>(f), std::forward<T>(x), s());
    }
    template <class F, class T, class = if_t<is_terminal, T>>
    static constexpr void invoke(F&& f, T&& x) {
        std::forward<F>(f)(std::forward<T>(x));
    }
};

// Invoker structure declaration: column specialization
template <>
struct invoker<option::access::column>
{
    // Call
    template <class F, class T, class = if_not_t<is_iterator, T>>
    constexpr void operator()(F&& f, T&& x) {
        constexpr std::size_t n = std::tuple_size<raw_t<T>>()();
        invoke(std::forward<F>(f), std::get<n / 2>(std::forward<T>(x)));
    }
    template <class F, class T, class... D, class = if_t<is_iterator, T>>
    constexpr void operator()(F&& f, T&& x, D...) {
        invoke(std::forward<F>(f), *std::forward<T>(x));
    }

    // Invocation
    template <class F, class T, class... D, class = if_t<is_container, T>>
    static constexpr void invoke(F&& f, T&& x, D...) {
        for (auto&& e: std::forward<T>(x)) {
            invoke(std::forward<F>(f), std::forward<decltype(e)>(e));
        } 
    }
    template <class F, class T, std::size_t... I, class = if_t<is_tuple, T>>
    static constexpr void invoke(F&& f, T&& x, std::index_sequence<I...>) {
        using dummy = int[];
        (void)dummy{1, (
            invoke(std::forward<F>(f), std::get<I>(std::forward<T>(x))),
        void(), int{})...};
    }
    template <class F, class T, class = if_t<is_tuple, T>>
    static constexpr void invoke(F&& f, T&& x, int = int{}) {
        using s = std::make_index_sequence<std::tuple_size<raw_t<T>>::value>;
        invoke(std::forward<F>(f), std::forward<T>(x), s());
    }
    template <class F, class T, class = if_t<is_terminal, T>>
    static constexpr void invoke(F&& f, T&& x) {
        std::forward<F>(f)(std::forward<T>(x));
    }
};
/* ************************************************************************** */



// ========================================================================== //
#endif // _UNPACK_META_BENCHMARK_HPP_INCLUDED
// ========================================================================== //

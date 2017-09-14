// ============================ UNPACK BENCHMARK ============================ //
// Project:         unpack
// Name:            unpack_benchmark.cpp
// Description:     Runs benchmarks on unpack
// Contributor(s):  Collin Gress [2017]
//                  Vincent Reverdy [2017]
// License:         BSD 3-Clause License
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
// Project sources
#include "unpack.hpp"
#include "unpack_alt_benchmark.hpp"
// Third-party libraries
#ifdef WITHGOOGLEBENCHMARK
#include "benchmark/benchmark_api.h"
#endif
// Miscellaneous
// ========================================================================== //



/* ******************************  BENCHMARKER ****************************** */
// Benchmarker structure declaration
template <option::tool>
struct benchmarker;

// Benchmarker structure definition: system specialization
template <>
struct benchmarker<option::tool::system>
{
    // Benchmark
    template <class F, class R, class M>
    double operator()(F&& function, R&& range, M&& measurements) {
        using clock_type = std::chrono::system_clock;
        using duration_type = std::chrono::duration<double>;
        typename clock_type::time_point tbegin;
        typename clock_type::time_point tend;
        typename clock_type::time_point t0;
        typename clock_type::time_point t1;
        tbegin = clock_type::now();
        for (auto&& m: std::forward<M>(measurements)) {
            t0 = clock_type::now();
            for (auto&& x: std::forward<R>(range)) {
                std::forward<F>(function)(std::forward<decltype(x)>(x));
            }
            t1 = clock_type::now();
            m = std::chrono::duration_cast<duration_type>(t1 - t0).count();
        }
        tend = clock_type::now();
        return std::chrono::duration_cast<duration_type>(tend - tbegin).count();
    }

};

// Benchmarker structure definition: chrono specialization
template <>
struct benchmarker<option::tool::chrono>
{
    // Benchmark
    template <class F, class R, class M>
    double operator()(F&& function, R&& range, M&& measurements) {
        using clock_type = std::chrono::steady_clock;
        using duration_type = std::chrono::duration<double>;
        typename clock_type::time_point tbegin;
        typename clock_type::time_point tend;
        typename clock_type::time_point t0;
        typename clock_type::time_point t1;
        tbegin = clock_type::now();
        for (auto&& m: std::forward<M>(measurements)) {
            t0 = clock_type::now();
            for (auto&& x: std::forward<R>(range)) {
                std::forward<F>(function)(std::forward<decltype(x)>(x));
            }
            t1 = clock_type::now();
            m = std::chrono::duration_cast<duration_type>(t1 - t0).count();
        }
        tend = clock_type::now();
        return std::chrono::duration_cast<duration_type>(tend - tbegin).count();
    }
};

// Benchmarker structure definition: google specialization
template <>
struct benchmarker<option::tool::google>
{
    // Benchmark
    template <class R, class F, class M>
    double operator()(R&&, F&&, M&&) {
        return 0;
    }
};

// Benchmarker structure definition: googles specialization
template <>
struct benchmarker<option::tool::googles>
{
    // Benchmark
    template <class R, class F, class M>
    double operator()(R&&, F&&, M&&) {
        return 0;
    }
};
/* ************************************************************************** */



/* *******************************  EXECUTOR ******************************** */
// Executor structure definition
template <
    class Options,
    template <class...> class Unpacker,
    class Initializer = random_initializer<>,
    class Finalizer = byte_checker
>
struct executor
{
    // Types
    using options_type = Options;
    using initializer_type = Initializer;
    using finalizer_type = Finalizer;
    using container_type = typename parameterized_container<
        options_type::orientation,
        options_type::container,
        typename options_type::type,
        Unpacker
    >::type;
    using invoker_type = invoker<options_type::access>;
    using operation_type = operation<options_type::complexity>;
    using benchmaker_type = benchmarker<options_type::tool>;

    // Lifecyle
    executor(
        std::size_t iterations,
        std::size_t container_size,
        const initializer_type& i = initializer_type(),
        const finalizer_type& f = finalizer_type()
    )
    : count(iterations)
    , size(container_size)
    , initializer(i)
    , finalizer(f)
    , timing(0)
    , measurements(count, 0) {
    }

    // Call benchmark
    void operator()() {
        auto function = [](auto&& x){
            invoker_type()(operation_type(), std::forward<decltype(x)>(x));
        };
        benchmaker_type benchmark;
        container_type container(size);
        initializer(container);
        timing = benchmark(function, container, measurements);
        finalizer(container);
    }

    // Members
    std::size_t count;
    std::size_t size;
    initializer_type initializer;
    finalizer_type finalizer;
    double timing;
    std::vector<double> measurements;
};
/* ************************************************************************** */



/* ********************************  PROGRAM ******************************** */
// Program structure definition
template <bool WithGoogle = true, bool WithAllTypes = true>
struct program
{
    // Types
    using c_t = char;
    using i_t = int;
    using u_t = unsigned int;
    using q_t = unsigned long long int;
    using f_t = float;
    using d_t = double;
    using l_t = long double;
    using s_t = std::string;
    using a_t = std::array<double, 3>;
    using v_t = std::vector<double>;
    using tool_selector_t = typename std::conditional<
        WithGoogle,
        option_selector<
            option::tool,
            option::tool::system,
            option::tool::chrono,
            option::tool::google,
            option::tool::googles
        >,
        option_selector<
            option::tool,
            option::tool::system,
            option::tool::chrono
        >
    >::type;
    using orientation_selector_t = option_selector<
        option::orientation,
        option::orientation::aos,
        option::orientation::soa
    >;
    using container_selector_t = option_selector<
        option::container,
        option::container::vector
    >;
    using type_selector_t = typename std::conditional<
        WithAllTypes,
        type_selector<
            ntuple<c_t, 1>, ntuple<c_t, 2>, ntuple<c_t, 3>, ntuple<c_t, 4>,
            ntuple<c_t, 5>, ntuple<c_t, 6>, ntuple<c_t, 7>, ntuple<c_t, 8>,
            ntuple<c_t, 9>, ntuple<c_t, 10>, ntuple<c_t, 11>, ntuple<c_t, 12>,
            ntuple<c_t, 13>, ntuple<c_t, 14>, ntuple<c_t, 15>, ntuple<c_t, 16>,
            ntuple<i_t, 1>, ntuple<i_t, 2>, ntuple<i_t, 3>, ntuple<i_t, 4>,
            ntuple<i_t, 5>, ntuple<i_t, 6>, ntuple<i_t, 7>, ntuple<i_t, 8>,
            ntuple<i_t, 9>, ntuple<i_t, 10>, ntuple<i_t, 11>, ntuple<i_t, 12>,
            ntuple<i_t, 13>, ntuple<i_t, 14>, ntuple<i_t, 15>, ntuple<i_t, 16>,
            ntuple<u_t, 1>, ntuple<u_t, 2>, ntuple<u_t, 3>, ntuple<u_t, 4>,
            ntuple<u_t, 5>, ntuple<u_t, 6>, ntuple<u_t, 7>, ntuple<u_t, 8>,
            ntuple<u_t, 9>, ntuple<u_t, 10>, ntuple<u_t, 11>, ntuple<u_t, 12>,
            ntuple<u_t, 13>, ntuple<u_t, 14>, ntuple<u_t, 15>, ntuple<u_t, 16>,
            ntuple<q_t, 1>, ntuple<q_t, 2>, ntuple<q_t, 3>, ntuple<q_t, 4>,
            ntuple<q_t, 5>, ntuple<q_t, 6>, ntuple<q_t, 7>, ntuple<q_t, 8>,
            ntuple<q_t, 9>, ntuple<q_t, 10>, ntuple<q_t, 11>, ntuple<q_t, 12>,
            ntuple<q_t, 13>, ntuple<q_t, 14>, ntuple<q_t, 15>, ntuple<q_t, 16>,
            ntuple<f_t, 1>, ntuple<f_t, 2>, ntuple<f_t, 3>, ntuple<f_t, 4>,
            ntuple<f_t, 5>, ntuple<f_t, 6>, ntuple<f_t, 7>, ntuple<f_t, 8>,
            ntuple<f_t, 9>, ntuple<f_t, 10>, ntuple<f_t, 11>, ntuple<f_t, 12>,
            ntuple<f_t, 13>, ntuple<f_t, 14>, ntuple<f_t, 15>, ntuple<f_t, 16>,
            ntuple<d_t, 1>, ntuple<d_t, 2>, ntuple<d_t, 3>, ntuple<d_t, 4>,
            ntuple<d_t, 5>, ntuple<d_t, 6>, ntuple<d_t, 7>, ntuple<d_t, 8>,
            ntuple<d_t, 9>, ntuple<d_t, 10>, ntuple<d_t, 11>, ntuple<d_t, 12>,
            ntuple<d_t, 13>, ntuple<d_t, 14>, ntuple<d_t, 15>, ntuple<d_t, 16>,
            ntuple<l_t, 1>, ntuple<l_t, 2>, ntuple<l_t, 3>, ntuple<l_t, 4>,
            ntuple<l_t, 5>, ntuple<l_t, 6>, ntuple<l_t, 7>, ntuple<l_t, 8>,
            ntuple<l_t, 9>, ntuple<l_t, 10>, ntuple<l_t, 11>, ntuple<l_t, 12>,
            ntuple<l_t, 13>, ntuple<l_t, 14>, ntuple<l_t, 15>, ntuple<l_t, 16>,
            ntuple<s_t, 1>, ntuple<s_t, 2>, ntuple<s_t, 3>, ntuple<s_t, 4>,
            ntuple<s_t, 5>, ntuple<s_t, 6>, ntuple<s_t, 7>, ntuple<s_t, 8>,
            ntuple<s_t, 9>, ntuple<s_t, 10>, ntuple<s_t, 11>, ntuple<s_t, 12>,
            ntuple<s_t, 13>, ntuple<s_t, 14>, ntuple<s_t, 15>, ntuple<s_t, 16>,
            std::tuple<a_t>, std::tuple<a_t, a_t>, std::tuple<a_t, a_t, a_t>,
            std::tuple<v_t>, std::tuple<v_t, v_t>, std::tuple<v_t, v_t, v_t>,
            std::tuple<c_t, c_t, u_t, q_t>, std::tuple<c_t, u_t, c_t, q_t>,
            std::tuple<i_t, u_t, c_t, c_t, s_t, f_t, d_t, v_t>
        >,
        type_selector<
            ntuple<i_t, 1>, ntuple<i_t, 2>, ntuple<i_t, 3>, ntuple<i_t, 4>,
            ntuple<d_t, 1>, ntuple<d_t, 2>, ntuple<d_t, 3>, ntuple<d_t, 4>,
            std::tuple<a_t>, std::tuple<a_t, a_t>, std::tuple<a_t, a_t, a_t>,
            std::tuple<v_t>, std::tuple<v_t, v_t>, std::tuple<v_t, v_t, v_t>,
            std::tuple<c_t, c_t, u_t, q_t>, std::tuple<c_t, u_t, c_t, q_t>,
            std::tuple<i_t, u_t, c_t, c_t, s_t, f_t, d_t, v_t>
        >
    >::type;
    using complexity_selector_t = option_selector<
        option::complexity,
        option::complexity::simple,
        option::complexity::complex
    >;
    using access_selector_t = option_selector<
        option::access,
        option::access::single,
        option::access::independent,
        option::access::combined,
        option::access::mono
    >;

    // Lifecycle
    program(int argc, char** argv)
    : filename(parse(make_command(argc, argv), "filename"))
    , os(parse(make_command(argc, argv), "os"))
    , cpu(parse(make_command(argc, argv), "cpu"))
    , ram(parse(make_command(argc, argv), "ram"))
    , compiler(parse(make_command(argc, argv), "compiler"))
    , optimization(parse(make_command(argc, argv), "optimization"))
    , tool(parse(make_command(argc, argv), "tool"))
    , orientation(parse(make_command(argc, argv), "orientation"))
    , container(parse(make_command(argc, argv), "container"))
    , type(parse(make_command(argc, argv), "type"))
    , complexity(parse(make_command(argc, argv), "complexity"))
    , access(parse(make_command(argc, argv), "access"))
    , count(to_size(parse(make_command(argc, argv), "count")))
    , size(to_size(parse(make_command(argc, argv), "size")))
    , timing()
    , measurements() {
        if (make_command(argc, argv).find("--help") != std::string::npos) {
            std::cout << make_help();
            std::exit(0);
        }
    }

    // Run
    template <class... Args>
    auto operator()(Args&&... args) {
        return tool_selector_t()([&](auto tool_v){
            return orientation_selector_t()([&](auto orientation_v){
                return container_selector_t()([&](auto container_v){
                    return type_selector_t()([&](auto type_v){
                        return complexity_selector_t()([&](auto complexity_v){
                            return access_selector_t()([&](auto access_v){
                                using config = options<
                                    decltype(tool_v)::value,
                                    decltype(orientation_v)::value,
                                    decltype(container_v)::value,
                                    decltype(type_v),
                                    decltype(complexity_v)::value,
                                    decltype(access_v)::value
                                >;
                                using executor_t = executor<config, unpack>;
                                executor_t exec(count, size);
                                exec(std::forward<Args>(args)...);
                                timing = exec.timing;
                                measurements = exec.measurements;
                            }, access);
                        }, complexity);
                    }, type);
                }, container);
            }, orientation);
        }, tool);
    }

    // Json
    std::string json() const {
        auto print_string = [](const std::string& p, const auto& x) {
            return std::string("\"") + p + "\": \"" + x + "\"";
        };
        auto print_number = [](const std::string& p, auto x) {
            std::ostringstream oss;
            oss << "\"" << p << "\": " << x;
            return oss.str();
        };
        auto print_array = [](const std::string& p, const auto& a) {
            std::ostringstream oss;
            oss << "\"" << p << "\": [";
            a.size() ? oss << a[0] : oss;
            for (std::size_t i = 1; i < a.size(); ++i) {
                oss << ", " << a[i];
            }
            oss << "]";
            return oss.str();
        };
        std::string text = "{";
        text += print_string("filename", filename) + ", ";
        text += print_string("os", os) + ", ";
        text += print_string("cpu", cpu) + ", ";
        text += print_string("ram", ram) + ", ";
        text += print_string("compiler", compiler) + ", ";
        text += print_string("optimization", optimization) + ", ";
        text += print_string("tool", tool) + ", ";
        text += print_string("orientation", orientation) + ", ";
        text += print_string("container", container) + ", ";
        text += print_string("type", type) + ", ";
        text += print_string("complexity", complexity) + ", ";
        text += print_string("access", access) + ", ";
        text += print_number("count", count) + ", ";
        text += print_number("size", size) + ", ";
        text += print_number("timing", timing) + ", ";
        text += print_array("measurements", measurements) + "}";
        return text;
    }

    // Parse an option
    static std::string parse(const std::string& com, const std::string& opt) {
        const std::string prefix = "\\s-{1,2}\\s*=?\\s*";
        const std::string first = "\\s*=?\\s*(\\\"([^\\\"]*)\\\"|";
        const std::string second = "\\'([^\\']*)\\'|([^\\s\\'\\\"]+))";
        const std::regex regex(prefix + opt + first + second);
        std::string result;
        std::sregex_iterator it(std::begin(com), std::end(com), regex);
        for (; it != std::sregex_iterator() && result.size() == 0; ++it) {
            for (std::size_t i = 2; i < (*it).size(); ++i) {
                result += (*it)[i];
            }
        }
        return result;
    }

    // To size
    static std::size_t to_size(const std::string& text) {
        return std::strtoull(text.c_str(), nullptr, 10);
    }

    // Make command
    static std::string make_command(int argc, char** argv) {
        static const std::string space = " ";
        std::string command = argv[0] + space;
        for (int i = 1; i < argc; ++i) {
            command += argv[i] + space ;
        }
        return command;
    }

    // Make help
    static std::string make_help() {
        std::string text;
        text += "Unpack Benchmark - C. Gress and V. Reverdy - 2017;\n";
        text += "Compilation: g++ -Wall -Wextra -pedantic -O3 -g -std=c++14 ";
        text += "unpack_benchmark.cpp -o unpack_benchark\n";
        text += "Options: -DWITHGOOGLE, -DALLTYPES, -DNDEBUG\n";
        text += "Execution: ./unpack_benchmark --parameter=\\\"value\\\"\n";
        text += "Parameters: --filename --os --cpu --ram --compiler ";
        text += "--optimization --tool --orientation --container --type ";
        text += "--complexity --access --count --size [--help]\n";
        text += "--filename: the filename for the output\n";
        text += "--os: a string representing the operating system\n";
        text += "--cpu: a string representing the cpu name\n";
        text += "--ram: a string representing information on memory\n";
        text += "--compiler: a string representing the compiler name\n";
        text += "--optimization: a string documenting the optimization level\n";
        text += "--tool: the tool used to benchmark (see below)\n";
        text += "--orientation: the memory layout (see below)\n";
        text += "--container: the container type (see below)\n";
        text += "--type: the element type (see below)\n";
        text += "--complexity: the complexity level (see below)\n";
        text += "--access: the access pattern (see below)\n";
        text += "--count: the number of benchmarks to run\n";
        text += "--size: the size of the container\n";
        text += "Options for --tool:\n";
        text += tool_selector_t().to_string();
        text += "Options for --orientation:\n";
        text += orientation_selector_t().to_string();
        text += "Options for --container:\n";
        text += container_selector_t().to_string();
        text += "Options for --type:\n";
        text += type_selector_t().to_string();
        text += "Options for --complexity:\n";
        text += complexity_selector_t().to_string();
        text += "Options for --access:\n";
        text += access_selector_t().to_string();
        return text;
    }

    // Members
    const std::string filename;
    const std::string os;
    const std::string cpu;
    const std::string ram;
    const std::string compiler;
    const std::string optimization;
    const std::string tool;
    const std::string orientation;
    const std::string container;
    const std::string type;
    const std::string complexity;
    const std::string access;
    const std::size_t count;
    const std::size_t size;
    double timing;
    std::vector<double> measurements;
};
/* ************************************************************************** */



// ================================== MAIN ================================== //
// Main function
int main(int argc, char* argv[])
{
    program<> benchmark(argc, argv);
    benchmark();
    std::cout << benchmark.json() << std::endl;
    return 0;
}
// ========================================================================== //

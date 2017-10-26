#include <cassert>
#include <cmath>
#include <thread>
#include "benchmark_utils.hpp"
#include "unpack.hpp"

template <class Clock = std::chrono::high_resolution_clock, class Opts>
double run(Opts&& opts) {
  using clock = Clock;
  typename clock::time_point tbegin;
  typename clock::time_point tend;
  std::chrono::duration<double> duration;
  volatile double tmp = 0;
  auto start_timing = [&tbegin]() {
    tbegin = clock::now();
  };
  tmp = unpack_benchmark::run_benchmark(std::forward<Opts>(opts), start_timing);
  if (std::isnan(tmp)) {
    std::cerr << tmp << std::endl;
  }
  tend = clock::now();
  duration = tend - tbegin;
  return duration.count();
}

int main(int argc, char* argv[]) {
  assert(argc >= 9);
  unpack_benchmark::opts o(argv, argc, "unpack_chrono_benchmark");
  std::chrono::duration<double> tsleep(o.time_limit - 1);
  std::thread terminator;
  o.print();  

  if (o.time_limit > 0) {
    terminator = std::thread([=](){
      std::this_thread::sleep_for(tsleep);
      std::exit(0);
    });
    while (true) {
      std::cout << run(o) << std::endl;
    }
    terminator.join();
  } else {
    std::cout << run(o) << std::endl;
  }

  return 0;
}



#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

using InputItem = int;
using Input = std::vector<InputItem>;
int solve_problem_1(const Input &inputs, int bus_time) {
  auto delta_to_bus =
      inputs | views::filter([](auto i) { return i > 0; }) |
      views::transform([bus_time](auto schedule) {
        return std::make_pair(schedule - (bus_time % schedule), schedule);
      }) |
      to<std::vector>;
  auto best = min_element(delta_to_bus);
  fmt::print("{}\n", delta_to_bus);
  fmt::print("{}\n", *best);
  return best->first * best->second;
}

uint64_t solve_problem_2(const Input &inputs, int bus_time) {
  auto requirements = views::zip(views::ints, inputs) |
                      views::filter([](auto i) { return i.second > 0; }) |
                      to<std::vector>;
  fmt::print("{}\n", requirements);
  actions::sort(requirements,
                [](auto a, auto b) { return a.second < b.second; });
  uint64_t increment = 1;
  uint64_t soln = 0;
  for (auto [ct, sch] : requirements) {
    for (uint64_t x = soln; true; x += increment) {
      if ((0 == (x + ct) % sch)) {
        increment *= sch;
        soln = x;
        fmt::print("- Found {} {} {} {} {} \n", ct, sch, x, increment, soln);
        break;
      }
    }
  }
  fmt::print("Found {} {} \n", soln, increment);
  return soln;
  for (uint64_t x = 0;; x += increment) {
    for (auto [ct, sch] : requirements) {
      increment = 1;
      if ((0 == (x + ct) % sch)) {
        increment *= sch;
      }
    }
    if (accumulate(requirements | views::transform([x](auto r) {
                     return int(0 != (x + r.first) % r.second);
                   }),
                   0) == 0)
      return x;
    // fmt::print("inc: {} x: {}\n", increment, x);
  }
  return 42;
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;
  std::string line;
  getline(input_file, line);
  int bus_time = std::stoi(line);
  getline(input_file, line);
  inputs = line |
           views::split(',')
           // | views::filter([](auto i) {
           //          auto s = i | to<std::string>;
           //          fmt::print(" filtering: {}\n", s);
           //          return s != "x";
           //        }) |
           | views::transform([](auto i) {
               auto s = i | to<std::string>;
               if (s == "x") {
                 return -1;
               }
               fmt::print(" to int: {}\n", s);
               return std::stoi(s);
             }) |
           to<Input>;
  fmt::print(" bus time: {}\n", bus_time);
  fmt::print("{}\n", inputs);

#if 0
  Input inputs = istream_range<std::string>(input_file) |
                 views::transform([](auto entry) {
                                  fmt::print("Line: {}\n", entry);
                   return entry | views::split(',') | views::filter([](auto e) {
                            fmt::print(" filtering: {}\n", e | to<std::string>);
                            return (e | to<std::string>) == "x";
                          }) |
                          views::transform([](auto e) {
                            fmt::print(" processing: {}\n", e | to<std::string>);
                            return std::stoi(e | to<std::string>);
                          });
                 }) |
                 to<Input>;
#endif

  fmt::print("{}\n", inputs);

  {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto answer = solve_problem_1(inputs, bus_time);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    fmt::print("Result = {}, resolved in {}usec\n", answer, duration);
  }

  {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto answer = solve_problem_2(inputs, bus_time);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    fmt::print("Result = {}, resolved in {}usec\n", answer, duration);
  }
}

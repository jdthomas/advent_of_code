#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

using InputItem = int64_t;
using Input = std::vector<InputItem>;
#define DBG 0

int64_t solve_problem_1(const Input &inputs) {
  std::unordered_map<int64_t, int64_t> last_seen;
  int64_t i = 0;
  int64_t speek = 0;

  for (; i < 2019; i++) {
    if (i < inputs.size()) {
      speek = inputs[i];
    }
    auto ls = last_seen.find(speek);
    if (ls == end(last_seen)) {
#if DBG
      fmt::print("** New number{}, next up 0 **\n", speek);
#endif
    }
    auto next_speek = (ls == end(last_seen)) ? 0 : (i - ls->second);
    last_seen[speek] = i;
#if DBG
    fmt::print("{}: speek {} last seen {}\n", i, speek, next_speek);
    fmt::print("{}\n", last_seen);
#endif
    speek = next_speek;
  }

  return speek;
}

int64_t solve_problem_2(const Input &inputs) {
  std::unordered_map<int64_t, int64_t> last_seen;
  int64_t i = 0;
  int64_t speek = 0;

  for (; i < 30000000 - 1; i++) {
    if (i < inputs.size()) {
      speek = inputs[i];
    }
    auto ls = last_seen.find(speek);
    if (ls == end(last_seen)) {
#if DBG
      fmt::print("** New number{}, next up 0 **\n", speek);
#endif
    }
    auto next_speek = (ls == end(last_seen)) ? 0 : (i - ls->second);
    last_seen[speek] = i;
#if DBG
    fmt::print("{}: speek {} last seen {}\n", i, speek, next_speek);
    fmt::print("{}\n", last_seen);
#endif
    speek = next_speek;
  }

  return speek;
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  std::string line;
  getline(input_file, line);
  Input inputs =
      line | views::split(',') |
      views::transform([](auto i) { return std::stoll(i | to<std::string>); }) |
      to<Input>();

  fmt::print("{}\n", inputs);

  {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto answer = solve_problem_1(inputs);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    fmt::print("Result = {}, resolved in {}usec\n", answer, duration);
  }

  {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto answer = solve_problem_2(inputs);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    fmt::print("Result = {}, resolved in {}usec\n", answer, duration);
  }
}

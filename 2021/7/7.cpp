#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
#include <charconv>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
using namespace ranges;

using InputItem = int64_t;
using Input = std::vector<InputItem>;

int64_t solve_problem_1(Input &inputs) {
  // InputItem avg_pos =
  //     std::round((double)ranges::accumulate(inputs, 0) / inputs.size());
  // fmt::print("avg: {}\n", avg_pos);
  auto median = inputs.begin() + inputs.size() / 2;
  ranges::nth_element(inputs.begin(), median, inputs.end());
  fmt::print("avg: {}\n", *median);
  return ranges::accumulate(
      inputs | ranges::views::transform(
                   [dest = *median](auto p) { return std::abs(p - dest); }),
      0);
}

uint64_t solve_problem_2(Input &inputs) {
  const auto min_cost_at_dest = [](const auto dest,
                                   const auto &inputs) -> int64_t {
    return ranges::accumulate(inputs | ranges::views::transform([dest](auto p) {
                                auto n = std::abs(p - dest);
                                return n * (n + 1) / 2;
                              }),
                              0);
  };

  const auto &[mn, mx] = ranges::minmax(inputs);
  auto costs = views::iota(mn, mx) | views::transform([&](auto dest) {
                 return min_cost_at_dest(dest, inputs);
               });
  return *ranges::min_element(costs);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::print("specify an input file please\n");
    return 1;
  }
  std::ifstream input_file(argv[1], std::ios_base::in);
  std::string line;
  getline(input_file, line);
  Input inputs = line | ranges::views::split(',') |
                 views::transform([](auto i) -> int64_t {
                   return stoll(i | to<std::string>);
                 }) |
                 ranges::to<Input>();

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

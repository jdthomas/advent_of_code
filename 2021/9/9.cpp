#include <fmt/core.h>
#include <fmt/ranges.h>

#include <bit>
#include <chrono>
#include <experimental/mdspan>
#include <fstream>
#include <iostream>
#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace stdex = std::experimental;
using namespace ranges;
using InputItem = uint8_t;
struct Input {
  std::vector<InputItem> data;
  int width = 0;
  int height = 0;
};

int64_t solve_problem_1(const Input &inputs) {
  auto s = stdex::mdspan(inputs.data.data(), inputs.height, inputs.width);
  auto is_minima = [&](auto i, auto j) {
    return (i <= 0 || (s(i - 1, j) > s(i, j))) &&
           (j <= 0 || (s(i, j - 1) > s(i, j))) &&
           (i >= s.extent(0) - 1 || (s(i + 1, j) > s(i, j))) &&
           (j >= s.extent(1) - 1 || (s(i, j + 1) > s(i, j)));
  };
  auto pos = ranges::views::cartesian_product(
      ranges::views::iota(0, static_cast<int>(s.extent(0))),
      ranges::views::iota(0, static_cast<int>(s.extent(1))));
  return ranges::accumulate(pos | views::transform([&](auto p) {
                              auto &[i, j] = p;
                              return is_minima(i, j) ? 1 + s(i, j) : 0;
                            }),
                            0, std::plus{});
}

int64_t solve_problem_2(const Input &inputs) { return 55; }

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::print("specify an input file please\n");
    return 1;
  }
  std::ifstream input_file(argv[1], std::ios_base::in);
  std::string line;

  Input inputs;
  while (getline(input_file, line)) {
    inputs.width = line.size();
    inputs.height++;
    auto blah = line |
                views::transform([](char c) -> uint8_t { return c - '0'; }) |
                ranges::to<std::vector>();
    copy(blah, back_inserter(inputs.data));
    fmt::print("{}\n", blah);
  }

  fmt::print("{} {} {} \n", inputs.data, inputs.width, inputs.height);

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

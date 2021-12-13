#include <fmt/core.h>
#include <fmt/ranges.h>

#include <bit>
#include <chrono>
#include <deque>
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

int64_t solve_problem_2(const Input &inputs) {
  auto s = stdex::mdspan(inputs.data.data(), inputs.height, inputs.width);
  std::vector<int> seen(inputs.height * inputs.width);
  auto m = stdex::mdspan(seen.data(), inputs.height, inputs.width);
  auto pos = ranges::views::cartesian_product(
      ranges::views::iota(0, static_cast<int>(s.extent(0))),
      ranges::views::iota(0, static_cast<int>(s.extent(1))));
  std::array<int64_t, 3> basin_sizes{};
  for (const auto &[i, j] : pos) {
    // for each position if it is not seen, do a BFS and save size of this basin
    // if in top 3.
    if (m(i, j) || s(i, j) == 9) continue;
    {
      std::deque<std::pair<int, int>> q{};
      q.emplace_back(i, j);
      int64_t basin_size = 0;
      while (!q.empty()) {
        auto cur = q.back();
        q.pop_back();
        // bounds check
        if (cur.first < 0 || cur.second < 0 || cur.first >= s.extent(0) ||
            cur.second >= s.extent(1))
          continue;
        // Check if we've seen this node
        if (m(cur.first, cur.second)) continue;
        // or it is a wall
        if (s(cur.first, cur.second) == 9) continue;
        basin_size += 1;
        m(cur.first, cur.second) = 1;
        // visit our neighbors
        q.emplace_back(cur.first + 1, cur.second);
        q.emplace_back(cur.first, cur.second + 1);
        q.emplace_back(cur.first - 1, cur.second);
        q.emplace_back(cur.first, cur.second - 1);
      }
      auto &me = *min_element(basin_sizes);
      if (basin_size >= me) {
        std::swap(me, basin_size);
      }
    }
  }

  return accumulate(basin_sizes, 1l, ranges::multiplies{});
}

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

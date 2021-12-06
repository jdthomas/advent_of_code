#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
#include <charconv>
#include <chrono>
#include <cmath>
#include <ctre.hpp>
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

// Favorite from day6
// https://twitter.com/TartanLlama/status/1467796731379257350?s=20
auto run_simulation(auto buckets, int days) {
  for (auto i = 0; i < days; ++i) {
    std::rotate(buckets.begin(), buckets.begin() + 1, buckets.end());
    buckets[6] += buckets[8];
  }

  return ranges::accumulate(buckets, 0ull);
}


uint64_t solve_problem_1(const Input &inputs, const int days = 80) {
  // std::vector<uint64_t> buckets(9);
  // for(auto i: inputs) {
  //   buckets[i]++;
  // }
  // auto r = run_simulation(buckets, 80);
  // return r;
  // Pretty sure there's a closed form solution here, but for the shit of it
  // lets bruit force .. so simulate we will...
  auto old_fish = inputs;
  for (const auto &day : ranges::views::iota(0, days)) {
    // fmt::print("After {} days: {}\n", day, old_fish);
    auto new_fish = ranges::views::repeat_n(
        8, ranges::count_if(old_fish, [](const auto &v) { return v == 0; }));
    old_fish = ranges::views::concat(
                   old_fish | ranges::views::transform([](const auto &v) {
                     return v > 0 ? v - 1 : 6;
                   }),
                   new_fish) |
               to<std::vector>;
  }

  return old_fish.size();
}

uint64_t recurrence_offspring(int days) {
  if(days < 0) {
    return 0;
  }
  return 1 + recurrence_offspring(days-7) + recurrence_offspring(days-9);
}
uint64_t recurrence_closed(int days) {
    // https://www.wolframalpha.com/input/?i=f%28x%29+%3D+f%28x-7%29+%2B+f%28x-9%29+%2B+1
    return pow(days, 9) - pow(days, 7) - 1;
}

uint64_t solve_problem_2(Input &inputs) {
  // std::vector<uint64_t> buckets(9);
  // for(auto i: inputs) {
  //   buckets[i]++;
  // }
  // return run_simulation(buckets, 256);
  // 
  // Alright, now it is way to big for brute force ... lets solve the closed
  // form for how many fish a single one will produce after a generation and
  // just sum that across the input fisheys.
  Input i{0};
  for (auto days = 1; days < 80; days++) {
    auto ooo = solve_problem_1(i, days);
    auto o = recurrence_closed(days - i[0] - 1);
    auto oo = recurrence_offspring(days - i[0] - 1); // 1 + pow(2, days / 6);// - pow(2, std::max(0, days - days*6) / 8);
    fmt::print("Closed?? (started at {}) {}: {} == {}  === {} --> {}\n", i[0], days, o,
               oo, ooo, oo - o);
  }
  auto old_fish = inputs;
  return old_fish.size();
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

  // {
  //   auto t1 = std::chrono::high_resolution_clock::now();
  //   auto answer = solve_problem_1(inputs);
  //   auto t2 = std::chrono::high_resolution_clock::now();
  //   auto duration =
  //       std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

  //   fmt::print("Result = {}, resolved in {}usec\n", answer, duration);
  // }

  {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto answer = solve_problem_2(inputs);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    fmt::print("Result = {}, resolved in {}usec\n", answer, duration);
  }
}

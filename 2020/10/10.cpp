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
int solve_problem_1(Input &inputs) {
  std::vector<int> delta;
  std::adjacent_difference(begin(inputs), end(inputs),
                           std::back_inserter(delta));
  actions::sort(delta);
  fmt::print("{}\n", delta);
  auto x =
      views::group_by(delta, [](auto lhs, auto rhs) { return lhs == rhs; }) |
      to<std::vector>();
  fmt::print("{}\n", x);
  auto y = x |
           views::transform([](auto g) { return distance(begin(g), end(g)); }) |
           to<std::vector>();
  fmt::print("{}\n", y);
  return y.front() * y.back();
}

uint64_t solve_problem_2(const Input &inputs) {
  std::vector<uint64_t> dp(
      inputs.size()); // table of "how many ways can I reach here"
  // For each adapter
  for (int i = 0; i < inputs.size(); i++) {
    fmt::print("input[{}] = {} (del {})\n", i, inputs[i],
               inputs[i] - inputs[i - 1]);
    // Count all the plugs we could have before us
    if (inputs[i] <= 3) {
      dp[i] += 1; // Add one for being reachable from the start
    }
    for (int j = i - 1; j >= 0 && inputs[i] - inputs[j] <= 3; j--) {
      dp[i] += dp[j];
      fmt::print("\tdp[{}] += dp[{}] -- {}\n", i, j, dp[i]);
    }
  }
  fmt::print("{}\n", dp);
  return dp.back();
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;

  std::copy(std::istream_iterator<InputItem>(input_file),
            std::istream_iterator<InputItem>(), std::back_inserter(inputs));
  actions::sort(inputs);
  inputs.push_back(inputs.back() + 3); // Add the computer
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

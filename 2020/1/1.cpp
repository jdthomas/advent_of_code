
// clang  -O3 -std=c++17 -g0 -Wall -Wextra -lstdc++ -lfmt -L$HOME/homebrew/lib
// -I$HOME/homebrew/include/ 1.cpp

#include <fstream>
#include <iostream>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;
#include <fmt/core.h>
#include <fmt/ranges.h>

using Input = std::vector<int>;

int solve_problem_1(const Input &inputs) {
  const int search_num = 2020;

  // TODO: How to represent w/ algorithms

  int result = 0;
  // Part 1
  for (int i = 0, j = inputs.size() - 1; i < j;) {
    const auto cur = inputs[i] + inputs[j];
    // fmt::print("cur: {} ({},{})\n", cur, i,j);
    if (cur < search_num) {
      ++i;
    } else if (cur > search_num) {
      --j;
    } else {
      result = inputs[i] * inputs[j];
      fmt::print("Found: {} * {} = {} \n", inputs[i], inputs[j], result);
      break;
    }
  }
  return result;
}

int solve_problem_2(const Input &inputs) {
  // part 2
  int result = 0;
  for (auto maybe : inputs) {
    // Copy of part 1, for each entry (should add a check that we don't use
    // maybe more than once, meh)
    const int search_num2 = 2020 - maybe;
    for (int i = 0, j = inputs.size() - 1; i < j;) {
      const auto cur = inputs[i] + inputs[j];
      if (cur < search_num2) {
        ++i;
      } else if (cur > search_num2) {
        --j;
      } else {
        result = maybe * inputs[i] * inputs[j];
        fmt::print("Found: {} * {} * {} = {} \n", maybe, inputs[i], inputs[j],
                   result);
        return result;
      }
    }
  }
  return result;
}

int main(int argc, char **argv) {
  Input inputs;
  std::fstream input_file(argv[1], std::ios_base::in);
  std::copy(std::istream_iterator<int>(input_file),
            std::istream_iterator<int>(), std::back_inserter(inputs));

  actions::sort(inputs);
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


// clang  -O3 -std=c++17 -g0 -Wall -Wextra -lstdc++ -lfmt -L$HOME/homebrew/lib
// -I$HOME/homebrew/include/ 1.cpp

#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;
#include <fmt/core.h>
#include <fmt/ranges.h>

int check_slope(const std::vector<std::string> &inputs, const int step_x,
                const int step_y) {
  int x = 0, y = 0;
  int collisions = 0;
  while (y < inputs.size()) {
    collisions += inputs[y][x] == '.' ? 0 : 1;
    x += step_x;
    x = x % inputs[0].size();
    y += step_y;
  }
  return collisions;
}
using Input = std::vector<std::string>;

int solve_problem_1(const Input &inputs) {
  const int step_x = 3, step_y = 1;
  auto collisions = check_slope(inputs, step_x, step_y);
  return collisions;
}
int solve_problem_2(const Input &inputs) {
  std::array<std::pair<int, int>, 5> slopes = {{
      {1, 1},
      {3, 1},
      {5, 1},
      {7, 1},
      {1, 2},
  }};
  auto result = accumulate(slopes | views::transform([&inputs](auto slope) {
                             auto [xstep, ystep] = slope;
                             return check_slope(inputs, xstep, ystep);
                           }),
                           1, std::multiplies<int>());
  // int result = 1;
  // for (auto slope : slopes) {
  //   auto [xstep, ystep] = slope;
  //   result *= check_slope(inputs, xstep, ystep);
  // }
  return result;
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;

  std::copy(std::istream_iterator<std::string>(input_file),
            std::istream_iterator<std::string>(), std::back_inserter(inputs));
  fmt::print("{}\n", inputs);

  // part 2

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

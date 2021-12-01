#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>

using InputItem = int64_t;
using Input = std::vector<InputItem>;

int solve_problem_1(const Input &inputs) {
  using namespace ranges;
  Input partial(inputs.size());
  adjacent_difference(inputs, partial,
                      [](auto a, auto b) { return a > b ? 1 : 0; });

  return accumulate(partial | views::drop(1), 0);
}

int solve_problem_2(const Input &inputs) {
  using namespace ranges;
  Input partial(inputs.size());
  adjacent_difference(
      inputs | views::sliding(3) |
          views::transform([](auto window) { return accumulate(window, 0); }),
      partial, [](auto a, auto b) { return a > b ? 1 : 0; });

  return accumulate(partial | views::drop(1), 0);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::print("specify an input file please\n");
    return 1;
  }
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs =
      ranges::istream_range<InputItem>(input_file) | ranges::to<Input>();

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

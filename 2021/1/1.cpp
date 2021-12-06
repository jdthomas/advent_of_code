#include <fmt/core.h>
#include <fmt/ranges.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
#include <vector>

using InputItem = int64_t;
using Input = std::vector<InputItem>;

/// My favorite result for day01:
/// https://twitter.com/tristanbrindle/status/1466116674554408971
constexpr auto count_increasing = [](int win_sz, auto const &input) {
  return std::inner_product(std::next(input.begin(), win_sz), input.end(),
                            input.begin(), 0, std::plus{}, std::greater{});
};

int solve_problem_1(const Input &inputs) {
  using namespace ranges;
  return distance(views::adjacent_filter(inputs, std::less{})) - 1;
}

int solve_problem_2(const Input &inputs) {
  using namespace ranges;
  return distance(inputs | views::sliding(3) |
                  views::transform(
                      [](auto window) { return accumulate(window, 0); }) |
                  views::adjacent_filter(std::less{})) -
         1;
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

    // assert(answer == count_increasing(1, inputs));
    fmt::print("Result = {}, resolved in {}usec\n", answer, duration);
  }

  {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto answer = solve_problem_2(inputs);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    // assert(answer == count_increasing(3, inputs));
    fmt::print("Result = {}, resolved in {}usec\n", answer, duration);
  }
}

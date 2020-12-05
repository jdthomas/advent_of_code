#include <fstream>
#include <iostream>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

using Input = std::vector<std::string>;

std::vector<unsigned int> to_seat_ids(const Input &inputs) {
  return inputs | views::transform([](auto seat_code) {
           auto row = accumulate(
               seat_code | views::take(7) |
                   views::transform([](auto c) { return c == 'F' ? 0 : 1; }),
               0u, [](auto a, auto b) { return b + (a << 1); });
           auto seat = accumulate(
               seat_code | views::drop(7) |
                   views::transform([](auto c) { return c == 'L' ? 0 : 1; }),
               0u, [](auto a, auto b) { return b + (a << 1); });
           // fmt::print("row {} seat {}\n", row, seat);
           return row * 8 + seat;
         }) |
         to<std::vector>;
}

int solve_problem_1(const Input &inputs) { return max(to_seat_ids(inputs)); }

int solve_problem_2(const Input &inputs) {
  auto seat_ids = to_seat_ids(inputs);
  actions::sort(seat_ids);

  // I think I can do this with adjacent_difference, but not intuative :/
  auto last = seat_ids[0];
  for (auto seat : seat_ids | views::drop(1)) {
    last++;
    while (last < seat) {
      fmt::print(" missing {} ({})\n", seat, last);
      return last;
      last++;
    }
  }
  // fmt::print("{}\n",seat_ids);
  return -1;
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;

  std::copy(std::istream_iterator<std::string>(input_file),
            std::istream_iterator<std::string>(), std::back_inserter(inputs));
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

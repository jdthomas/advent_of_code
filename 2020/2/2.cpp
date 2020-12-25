
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

struct Policy {
  int min, max;
  char letter;
};

template <> struct fmt::formatter<Policy> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(Policy const &policy, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "{0}-{1} {2}", policy.min, policy.max,
                          policy.letter);
  }
};
using Input = std::vector<std::pair<Policy, std::string>>;

int solve_problem_1(const Input &inputs) {
  // Count the valid strings
  auto valid = accumulate(inputs | views::transform([](auto input) {
                            auto [p, pw] = input;
                            // count occurances of our letter in the string
                            auto ct = accumulate(
                                pw | views::transform([input](auto c) {
                                  return input.first.letter == c ? 1 : 0;
                                }),
                                0);
                            // apply policy to the count
                            return (p.min <= ct && ct <= p.max) ? 1 : 0;
                          }),
                          0);

  return valid;
}
int solve_problem_2(const Input &inputs) {
  // Count the valid strings
  auto valid = accumulate(
      inputs | views::transform([](auto input) {
        auto [p, pw] = input;
        // apply policy to the count
        return (pw[p.min - 1] == p.letter && pw[p.max - 1] != p.letter ||
                pw[p.min - 1] != p.letter && pw[p.max - 1] == p.letter)
                   ? 1
                   : 0;
      }),
      0);
  return valid;
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;

  Policy policy{};
  std::string sep;
  std::string password;

  // TODO: better parsing code?
  while (input_file >> policy.min >> policy.max >> policy.letter >> sep >>
         password) {
    // Hack, just abs the max, it parses the - as a minus .. meh
    policy.max = -policy.max;
    inputs.emplace_back(std::make_pair(policy, password));
  }

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

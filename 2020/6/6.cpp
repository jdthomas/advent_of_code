#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

using InputItem = std::vector<std::vector<char>>;
using Input = std::vector<InputItem>;
int solve_problem_1(const Input &inputs) {
  return accumulate(inputs | views::transform(views::join) |
                        views::transform([](auto group) {
                          // fmt::print("g:{}\n", group);
                          std::unordered_set s(begin(group), end(group));
                          return s.size();
                        }),
                    0);
}

int solve_problem_2(const Input &inputs) {
  auto alphabet =
      std::string("abcdefghijklmnopqrstuvwxyz") | to<std::vector<char>>;
  return accumulate(
      inputs | views::transform([alphabet](auto group) {
        // oops, need to sort for std set_intersection to work :P
        auto sorted_group = group | views::transform([](auto s) {
                              actions::sort(s);
                              return s;
                            });
        return accumulate(sorted_group, alphabet, [](auto a, auto b) {
          std::vector<char> c;
          std::set_intersection(begin(a), end(a), begin(b), end(b),
                                std::back_inserter(c));
          // fmt::print("a:{}\nb:{}\n   -> c:{}\n", a, b, c);
          return c;
        });
      }) | views::transform([](auto s) {
        // fmt::print("\n");
        return s.size();
      }),
      0);
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;
  InputItem current;
  std::string line;
  while (getline(input_file, line)) {
    fmt::print("line: {}", line);
    if (line.empty()) {
      // empty lines delimante entries
      inputs.push_back(current);
      current.clear();
    } else {
      auto set_of_chars = line | to<std::vector<char>>;
      current.push_back(set_of_chars);
    }
  }
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

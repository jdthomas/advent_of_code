#include <fmt/core.h>
#include <fmt/ranges.h>

#include <bit>
#include <chrono>
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
using InputItem = std::string;
using Input = std::vector<InputItem>;

constexpr auto is_open_char = [](auto c) {
  switch (c) {
    case '{':
    case '[':
    case '(':
    case '<':
      return true;
  }
  return false;
};
constexpr auto is_correct_pair = [](auto o, auto c) {
  return (c == ')' && (c - o) == 1) || (c - o) == 2;
};
constexpr auto bad_score = [](auto c) {
  constexpr std::array<std::pair<char, int>, 4> scores{
      {{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}}};
  auto s = find_if(scores, [&](const auto &p) {
    auto &[cc, ss] = p;
    return cc == c;
  });
  assert(s != scores.end());
  return s->second;
};

constexpr auto autocomplete_score = [](auto c) {
  constexpr std::array<std::pair<char, int>, 4> scores{
      {{'(', 1}, {'[', 2}, {'{', 3}, {'<', 4}}};
  auto s = find_if(scores, [&](const auto &p) {
    auto &[cc, ss] = p;
    return cc == c;
  });
  assert(s != scores.end());
  return s->second;
};
constexpr auto command_to_score = [](auto line) -> std::pair<int64_t, int64_t> {
  std::vector<char> s;
  for (auto c : line) {
    if (is_open_char(c)) {
      s.push_back(c);
    } else {
      auto opener = s.back();
      s.pop_back();
      if (!is_correct_pair(opener, c)) {
        // fmt::print("bad: {} != {}, {}\n", opener, c, bad_score(c));
        return std::make_pair(bad_score(c), 0);
      }
    }
  }
  // fmt::print("remain: {}\n", s);
  return std::make_pair(
      0, accumulate(s | views::reverse | views::transform([](auto c) {
                      return autocomplete_score(c);
                    }),
                    0ll, [](auto acc, auto cur) { return 5 * acc + cur; }));
};
int64_t solve_problem_1(const Input &inputs) {
  return accumulate(inputs | views::transform([](auto c) {
                      return command_to_score(c).first;
                    }),
                    0);
}

int64_t solve_problem_2(const Input &inputs) {
  auto incomplete_commands = views::filter(
      inputs, [](auto line) { return command_to_score(line).first == 0; });
  auto inc_scores = incomplete_commands | views::transform([](auto line) {
                      return command_to_score(line).second;
                    }) |
                    to<std::vector>;

  // fmt::print("{}\n", inc_scores);

  auto median = inc_scores.begin() + inc_scores.size() / 2;
  ranges::nth_element(inc_scores.begin(), median, inc_scores.end());
  return *median;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::print("specify an input file please\n");
    return 1;
  }
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs =
      ranges::istream_range<InputItem>(input_file) | ranges::to<Input>();

  fmt::print("{} \n", inputs);

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

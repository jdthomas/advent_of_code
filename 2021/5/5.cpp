#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
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

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};
using BoardMapType = std::unordered_map<std::pair<int, int>, int, pair_hash>;

struct InputItem {
  std::pair<int, int> begin;
  std::pair<int, int> end;
};
using Input = std::vector<InputItem>;

auto segment_to_points =
    [](const auto &segment) -> std::vector<std::pair<int, int>> {
  // Meh, turn these into vectors because I cannot return different range
  // types conditionally.. Probably something better to do??
  if (segment.begin.first == segment.end.first) {
    return ranges::views::zip(
               ranges::views::repeat(0),
               ranges::views::iota(
                   0,
                   std::abs(segment.end.second - segment.begin.second) + 1)) |
           ranges::to<std::vector>;
  } else if (segment.begin.second == segment.end.second) {
    return ranges::views::zip(
               ranges::views::iota(
                   0, std::abs(segment.end.first - segment.begin.first) + 1),
               ranges::views::repeat(0)) |
           ranges::to<std::vector>;

  } else {
    return ranges::views::zip(
               ranges::views::iota(
                   0, std::abs(segment.begin.first - segment.end.first) + 1),
               ranges::views::iota(
                   0,
                   std::abs(segment.begin.second - segment.end.second) + 1)) |
           ranges::to<std::vector>;
  }
};

auto segment_apply_offset = [](const auto &segment, const auto &pos) {
  return std::make_pair(
      segment.begin.first +
          std::copysign(pos.first, segment.end.first - segment.begin.first),
      segment.begin.second +
          std::copysign(pos.second, segment.end.second - segment.begin.second));
};

void print_board(const BoardMapType &board_map) {
  fmt::print("board: {}\n", board_map);
  for (int j = 0; j <= 9; j++) {
    for (int i = 0; i <= 9; i++) {
      auto v = board_map.find(std::make_pair(i, j));
      if (v == board_map.end()) {
        fmt::print(".");
      } else {
        fmt::print("{}", v->second);
      }
    }
    fmt::print("\n");
  }
}

int64_t solve_problem_1(const Input &inputs) {
  // std::vector<int> board_map(map_size.first*map_size.second);
  // collect line segments into board_map
  BoardMapType board_map;
  for (const auto &segment :
       inputs | ranges::views::filter([](const auto &seg) {
         // Only accept horizontal or vertical segments
         return seg.begin.first == seg.end.first ||
                seg.begin.second == seg.end.second;
       })) {
    auto points = segment_to_points(segment);
    for (const auto &pos : points) {
      auto p = segment_apply_offset(segment, pos);
      // fmt::print("(seg: {}->{}) .. pos: {} -> p: {}\n", segment.begin,
      //            segment.end, pos, p);
      board_map[p] += 1;
    }
  }
  // print_board(board_map);
  return ranges::count_if(board_map,
                          [](const auto &p) { return p.second >= 2; });
}

int solve_problem_2(Input &inputs) {
  // std::vector<int> board_map(map_size.first*map_size.second);
  // collect line segments into board_map
  BoardMapType board_map;
  for (const auto &segment : inputs) {
    auto points = segment_to_points(segment);
    for (const auto &pos : points) {
      auto p = segment_apply_offset(segment, pos);
      board_map[p] += 1;
    }
  }
  // print_board(board_map);
  return ranges::count_if(board_map,
                          [](const auto &p) { return p.second >= 2; });
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::print("specify an input file please\n");
    return 1;
  }
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;

  std::string line;
  while (getline(input_file, line)) {
    static constexpr auto pattern =
        ctll::fixed_string{"([0-9]*),([0-9]*) -> ([0-9]*),([0-9]*)"};
    auto m = ctre::match<pattern>(line);
    auto x1 = m.get<1>().to_string();
    auto y1 = m.get<2>().to_string();
    auto x2 = m.get<3>().to_string();
    auto y2 = m.get<4>().to_string();

    auto a = std::make_pair(stoll(x1), stoll(y1));
    auto b = std::make_pair(stoll(x2), stoll(y2));
    inputs.emplace_back(InputItem{a, b});
  }
  // fmt::print("{}\n", inputs);

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

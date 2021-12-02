#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>

enum class Direction {
  FORWARD,
  UP,
  DOWN,
};

using InputItem = std::pair<Direction, int64_t>;
using Input = std::vector<InputItem>;
using Position = std::pair<int64_t, int64_t>;

int64_t solve_problem_1(const Input &inputs) {
  using namespace ranges;
  auto position = ranges::accumulate(
      inputs, Position{0, 0},
      [](const auto &a, const auto &b) {
        return std::make_pair(a.first + b.first, a.second + b.second);
      },
      [](const auto &p) -> std::pair<int64_t, int64_t> {
        auto &[dir, dist] = p;
        switch (dir) {
        case Direction::FORWARD:
          return {dist, 0};
        case Direction::UP:
          return {0, -dist};
        case Direction::DOWN:
          return {0, dist};
        }
        return {0, 0};
      });
  return position.first * position.second;
}

int solve_problem_2(const Input &inputs) {
  using namespace ranges;
  int64_t aim{0};
  auto position = ranges::accumulate(
      inputs, Position{0, 0},
      [](const auto &a, const auto &b) {
        return std::make_pair(a.first + b.first, a.second + b.second);
      },
      [&aim](const auto &p) -> std::pair<int64_t, int64_t> {
        auto &[dir, dist] = p;
        switch (dir) {
        case Direction::FORWARD:
          return {dist, aim * dist};
        case Direction::UP:
          aim -= dist;
          return {0, 0};
        case Direction::DOWN:
          aim += dist;
          return {0, 0};
        }
        return {0, 0};
      });
  return position.first * position.second;
}

Direction parse_dir(const std::string &direction) {
  if ("forward" == direction) {
    return Direction::FORWARD;
  } else if ("up" == direction) {
    return Direction::UP;
  } else if ("down" == direction) {
    return Direction::DOWN;
  }
  throw std::runtime_error("Unknown direction");
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::print("specify an input file please\n");
    return 1;
  }
  std::ifstream input_file(argv[1], std::ios_base::in);
  std::string direction;
  int64_t distance;
  Input inputs;
  while (input_file >> direction >> distance) {
    inputs.emplace_back(std::make_pair(parse_dir(direction), distance));
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

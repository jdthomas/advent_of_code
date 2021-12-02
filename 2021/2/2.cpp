#include <chrono>
#include <fstream>
#include <iostream>
#include <variant>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>

struct CmdImpl {
  int64_t dist;
};
struct Forward : CmdImpl {};
struct Up : CmdImpl {};
struct Down : CmdImpl {};
using Command = std::variant<Forward, Up, Down>;

template <class... Ts> struct overload : Ts... { using Ts::operator()...; };
template <class... Ts> overload(Ts...) -> overload<Ts...>;

using InputItem = Command;
using Input = std::vector<InputItem>;
using Position = std::pair<int64_t, int64_t>;

int64_t solve_problem_1(const Input &inputs) {
  auto position = ranges::accumulate(
      inputs, Position{0, 0},
      [](const auto &cur, const auto &nxt) {
        return std::make_pair(cur.first + nxt.first, cur.second + nxt.second);
      },
      [](const Command &p) {
        return std::visit(overload{
                              // clang-format off
                              [](Forward c) -> Position { return {c.dist, 0}; },
                              [](Up c) -> Position { return {0, -c.dist}; },
                              [](Down c) -> Position { return {0, c.dist}; },
                              // clang-format on
                          },
                          p);
      });
  return position.first * position.second;
}

int solve_problem_2(const Input &inputs) {
  using DistAimUpdate = std::pair<int64_t, int64_t>;
  auto position = ranges::accumulate(
      inputs, std::pair<uint64_t, Position>{0, {0, 0}},
      [](const auto &cur, const auto &nxt) -> std::pair<uint64_t, Position> {
        // Takes current aim/position as first arg, distance/aim update as
        // second and applies it.
        auto &[cur_aim, c] = cur;
        auto aim = cur_aim + nxt.second;
        return {aim, {c.first + nxt.first, c.second + nxt.first * aim}};
      },
      [](const Command &p) {
        // Emit a pair of distance / aim updates
        return std::visit(overload{
                              // clang-format off
                        [](Forward c) -> DistAimUpdate { return {c.dist, 0}; },
                        [](Up c) -> DistAimUpdate { return {0, -c.dist}; },
                        [](Down c) -> DistAimUpdate { return {0, c.dist}; },
                              // clang-format on
                          },
                          p);
      });
  return position.second.first * position.second.second;
}

Command cmd_factory(const std::string &direction, int64_t dist) {
  if ("forward" == direction) {
    return Forward{dist};
  } else if ("up" == direction) {
    return Up{dist};
  } else if ("down" == direction) {
    return Down{dist};
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
    inputs.emplace_back(cmd_factory(direction, distance));
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

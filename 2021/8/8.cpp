#include <fmt/core.h>
#include <fmt/ranges.h>

#include <bit>
#include <chrono>
#include <fstream>
#include <iostream>
#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

using namespace ranges;
using InputItem = std::pair<std::vector<uint8_t>, std::vector<uint8_t>>;
using Input = std::vector<InputItem>;

auto popcount = [](auto v) { return /*std::popcount*/ __builtin_popcount(v); };

int64_t solve_problem_1(const Input &inputs) {
  // 1: 2, 4: 4, 7: 3, 8: 7
  const std::unordered_set<int> uniq_segments{2, 4, 3, 7};
  return accumulate(
      inputs | views::transform([&](const auto &wo) {
        auto &[wire, outputs] = wo;
        return distance(
            outputs | views::transform([](auto o) { return popcount(o); }) |
            views::filter([&](auto p) { return uniq_segments.count(p) != 0; }));
        // return accumulate(
        //     outputs | views::transform([&](const auto &o) {
        //       return uniq_segments.count(popcount(o)) ? 1ll : 0ll;
        //     }),
        //     0ll);
      }),
      0ll);
}

int64_t solve_problem_2(const Input &inputs) {
  const std::unordered_map<uint8_t, uint8_t> uniq_segments{
      {2, 1}, {4, 4}, {3, 7}, {7, 8}};  // mapping of popcnt -> value
  const auto contains = [](auto v, auto c) -> bool { return (v & c) == c; };
  const auto build_wire_map = [&](const auto &wire) {
    std::array<uint8_t, 10> wire_map{};
    while (count(wire_map, 0) != 0) {
      // fmt::print("mapping: {}\n", wire_map);
      for (auto &w : wire) {
        auto p = popcount(w);
        auto u = uniq_segments.find(p);
        if (u != uniq_segments.end()) {
          wire_map[u->second] = w;
        }
        switch (p) {
          case 5:
            // fmt::print("One of: 2,3,5 .. {}\n", w);
            if (wire_map[7] && contains(w, wire_map[7])) {
              wire_map[3] = w;
            }
            if (wire_map[6] && contains(wire_map[6], w)) {
              wire_map[5] = w;
            }
            if (wire_map[3] && wire_map[5] && w != wire_map[3] &&
                w != wire_map[5]) {
              wire_map[2] = w;
            }
            break;
          case 6:
            // fmt::print("One of: 0,6,9 .. {}\n", w);
            if (wire_map[4] && contains(w, wire_map[4])) {
              wire_map[9] = w;
            }
            if (wire_map[7] && !contains(w, wire_map[7])) {
              wire_map[6] = w;
            }
            if (wire_map[9] && wire_map[9] != w && wire_map[6] &&
                wire_map[6] != w) {
              wire_map[0] = w;
            }
            break;
        }
      }
    }
    return wire_map;
  };
  return accumulate(
      inputs | views::transform([&](const auto &wo) {
        const auto &[wire, output] = wo;
        auto wire_map = build_wire_map(wire);
        // fmt::print("Final deduction: {}\n", wire_map);

        return accumulate(
            output | views::transform([&](const auto &o) -> uint64_t {
              auto f = ranges::find(wire_map, o);
              // Check that found to work around the supurious empty
              // string from poor parsing code
              return f != wire_map.end() ? distance(wire_map.begin(), f) : 0;
            }),
            0ll, [](auto acc, auto x) { return 10 * acc + x; });
      }),
      0ll, std::plus{});
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::print("specify an input file please\n");
    return 1;
  }
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;
  std::string l;
  auto label_to_byte = [](const auto &label) {
    return accumulate(label | views::transform([](auto c) -> uint8_t {
                        if (c < 'a' || c > 'z') return 0;
                        return 1 << (c - 'a');
                      }),
                      0, std::bit_or{});
  };
  auto labels_to_bytes = [&](const auto &labels) {
    return labels | views::split(' ') | views::transform(label_to_byte);
  };
  while (getline(input_file, l)) {
    auto split_input = l | views::split('|');
    auto labeled_wires = split_input.front() | to<std::string>;
    auto labeled_outputs =
        (split_input | views::drop(1)).front() | to<std::string>;
    auto wires = labels_to_bytes(labeled_wires) | to<std::vector<uint8_t>>;
    auto outputs = labels_to_bytes(labeled_outputs) | to<std::vector<uint8_t>>;
    // fmt::print("{} | {}\n", labeled_wires, labeled_outputs);
    // fmt::print("{} | {}\n", wires, outputs);
    auto p = std::make_pair(std::move(wires), std::move(outputs));
    // fmt::print("{}\n", p);
    inputs.push_back(std::move(p));
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

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
  // 1: 2
  // 4: 4
  // 7: 3
  // 8: 7
  const std::unordered_set<int> uniq_segments{2, 4, 3, 7};
  // TODO: this would probalby read cleaner if i transform to popcount, and
  // filter by in set and return size of that range. sum over wire | output pair
  return accumulate(inputs | views::transform([&](const auto &wo) {
                      auto &[wire, outputs] = wo;
                      // sum over each group in outputs
                      return accumulate(
                          outputs | views::transform([&](const auto &o) {
                            return uniq_segments.count(popcount(o)) ? 1ll : 0ll;
                          }),
                          0ll);
                    }),
                    0ll);
  return 44;
}

int64_t solve_problem_2(const Input &inputs) {
  const std::unordered_map<uint8_t, uint8_t> uniq_segments{
      {2, 1}, {4, 4}, {3, 7}, {7, 8}};  // mapping of popcnt -> value
  auto contains = [](auto v, auto c) -> bool { return (v & c) == c; };
  std::unordered_set<uint8_t> seen;
  auto sum_solution = 0;
  // Until we have the full mapping
  for (auto &[wire, output] : inputs) {
    std::unordered_map<uint8_t, uint8_t> deduced_wire_mapping{};
    while (deduced_wire_mapping.size() != 10) {
      fmt::print("mapping: {}\n", deduced_wire_mapping);
      for (auto &w : wire) {
        seen.insert(w);
        auto p = popcount(w);
        auto u = uniq_segments.find(p);
        if (u != uniq_segments.end()) {
          deduced_wire_mapping[u->second] = w;
        }
        switch (p) {
          case 5:
            fmt::print("One of: 2.3.5 .. {}\n", w);
            // one of: 2, 3, 5
            {
              auto val7 = deduced_wire_mapping.find(7);
              if (val7 != deduced_wire_mapping.end() &&
                  contains(w, val7->second)) {
                deduced_wire_mapping[3] = w;
              }
              auto val6 = deduced_wire_mapping.find(6);
              if (val6 != deduced_wire_mapping.end() &&
                  contains(val6->second, w)) {
                deduced_wire_mapping[5] = w;
              }
              auto val3 = deduced_wire_mapping.find(3);
              auto val5 = deduced_wire_mapping.find(5);
              if (val3 != deduced_wire_mapping.end() &&
                  val5 != deduced_wire_mapping.end() && w != val3->second &&
                  w != val5->second) {
                deduced_wire_mapping[2] = w;
              }
            }
            break;
          case 6:
            fmt::print("One of: 0,6,9 .. {}\n", w);
            // one of: 0, 6, 9
            {
              auto val4 = deduced_wire_mapping.find(4);
              if (val4 != deduced_wire_mapping.end() &&
                  contains(w, val4->second)) {
                deduced_wire_mapping[9] = w;
              }
              auto val7 = deduced_wire_mapping.find(7);
              if (val7 != deduced_wire_mapping.end() &&
                  !contains(w, val7->second)) {
                deduced_wire_mapping[6] = w;
              }
              auto val9 = deduced_wire_mapping.find(9);
              auto val6 = deduced_wire_mapping.find(6);
              if (val9 != deduced_wire_mapping.end() && val9->second != w &&
                  val6 != deduced_wire_mapping.end() && val6->second != w) {
                deduced_wire_mapping[0] = w;
              }
            }
            break;
        }
      }
    }
    fmt::print("Final deduction: {}\n", deduced_wire_mapping);
    fmt::print("seen: {}\n", seen);

    //
    auto r = accumulate(
        output | views::transform([&](const auto &o) -> uint64_t {
          // Ugh, built that backwards...
          auto f = ranges::find_if(deduced_wire_mapping,
                                   [&](auto kv) { return kv.second == o; });
          fmt::print("o={} -- {} {}:{}\n", o, (f == deduced_wire_mapping.end()),
                     (f != deduced_wire_mapping.end() ? f->first : 0),
                     (f != deduced_wire_mapping.end() ? f->second : 0));
          // assert(f!=deduced_wire_mapping.end());
          if (f == deduced_wire_mapping.end()) return 0;
          return f->first;
          // return deduced_wire_mapping[o];
        }),
        0ll, [](auto acc, auto x) { return 10 * acc + x; });
    sum_solution += r;
  }
  return sum_solution;
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
    auto labeled_wires = (l | views::split('|')).front() | to<std::string>;
    auto labeled_outputs =
        (l | views::split('|') | views::drop(1)).front() | to<std::string>;
    auto wires = labels_to_bytes(labeled_wires) | to<std::vector<uint8_t>>;
    auto outputs = labels_to_bytes(labeled_outputs) | to<std::vector<uint8_t>>;
    fmt::print("{} | {}\n", labeled_wires, labeled_outputs);
    fmt::print("{} | {}\n", wires, outputs);
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

#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <variant>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

struct MaskUpdate {
  MaskUpdate(uint64_t m, uint64_t v) : mask(m), valid(v){};
  uint64_t mask;
  uint64_t valid;
};

struct MemWrite {
  MemWrite(uint64_t a, uint64_t v) : address(a), value(v){};
  uint64_t address;
  uint64_t value;
};

template <> struct fmt::formatter<MaskUpdate> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(MaskUpdate const &cmd, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "MaskUpdate {0} ^ {1}", cmd.mask,
                          cmd.valid);
  }
};
template <> struct fmt::formatter<MemWrite> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(MemWrite const &cmd, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "MemWrite {0}: {1}", cmd.address,
                          cmd.value);
  }
};
using InputItem = std::variant<MaskUpdate, MemWrite>;
template <> struct fmt::formatter<InputItem> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(InputItem const &cmd, FormatContext &ctx) {
    auto idx = cmd.index();
    if (idx == 0) {
      auto mw = std::get<0>(cmd);
      return fmt::format_to(ctx.out(), "{0}", mw);
    } else if (idx == 1) {
      auto mu = std::get<1>(cmd);
      return fmt::format_to(ctx.out(), "{0}", mu);
    }
    return fmt::format_to(ctx.out(), "{0}", "oops");
    ;
  }
};

using Input = std::vector<InputItem>;

uint64_t solve_problem_1(const Input &inputs) {
  uint64_t cur_mask = 0x0, cur_mask_valid = 0x0;
  std::unordered_map<uint64_t, uint64_t> memory;
  for (auto input : inputs) {
    // std::visit(overloaded {
    if (input.index() == 0) {
      [&](MaskUpdate &update) {
        cur_mask = update.mask;
        cur_mask_valid = update.valid;
      }(std::get<0>(input));
    }
    if (input.index() == 1) {
      [&](MemWrite &w) {
        auto actual_val = w.value & ~cur_mask_valid | cur_mask & cur_mask_valid;
        memory[w.address] = actual_val;
      }(std::get<1>(input));
    }
    //},input);
  }
  fmt::print("{}\n", memory);
  return accumulate(memory | views::values, 0ull);
}

int solve_problem_2(const Input &inputs) { return 42; }

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  auto const r_mem = std::regex("mem\\[([0-9]*)\\] = ([0-9]*)");
  auto const r_mask = std::regex("mask = ([X0-1]*)");
  Input inputs;
  std::string line;
  while (getline(input_file, line)) {
    std::smatch match;
    std::regex_match(line, match, r_mem);
    // fmt::print("line {} match size {}\n", line, match.size());
    if (match.size() >= 3) {
      fmt::print("mem: addr={} val={}\n", match[1].str(), match[2].str());
      auto address = std::stoull(std::string(match[1].str()));
      auto value = std::stoull(std::string(match[2].str()));
      inputs.push_back(MemWrite{address, value});
    }
    std::regex_match(line, match, r_mask);
    fmt::print("line {} match size {}\n", line, match.size());
    if (match.size() >= 2) {
      fmt::print("mask: val={}\n", match[1].str());
      std::string s = match[1].str();
      auto valid = accumulate(
          s | views::transform([](auto b) { return b != 'X' ? 1 : 0; }), 0ull,
          [](auto a, auto b) { return a << 1 | b; });
      auto value = accumulate(
          s | views::transform([](auto b) { return b == '1' ? 1 : 0; }), 0ull,
          [](auto a, auto b) { return a << 1 | b; });
      fmt::print("s {} val {:x}\n", s, valid);
      inputs.push_back(MaskUpdate{value, valid});
    }
    // if("mem" == (entry | views::take(3) | to<std::string>)) {
    // auto address = std::stoi(entry | views::drop(4) | to<std::string>);
    // fmt::print("Mem: addr{}\n", address);
    // }
    // return std::make_pair(Command::MEM_WRITE, 42);
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

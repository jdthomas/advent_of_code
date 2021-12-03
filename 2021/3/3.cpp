#include <chrono>
#include <fstream>
#include <iostream>
#include <variant>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>

template <class... Ts> struct overload : Ts... { using Ts::operator()...; };
template <class... Ts> overload(Ts...) -> overload<Ts...>;

using InputItem = std::string;
using Input = std::vector<InputItem>;

int64_t solve_problem_1(const Input &inputs) {
  using namespace ranges;
  size_t bit_width = inputs.front().size();
  size_t mask = ((1 << bit_width) - 1);

  auto digit_c_to_i = [](auto d) -> int{ return d - '0'; };
  auto pairwise_add = [](const auto &a, const auto &b) {
    return views::transform(a, b, std::plus<int>{});
  };
  auto bit_counts = accumulate(
      inputs | views::transform([&](auto entry) {
        return entry | views::transform(digit_c_to_i) |
               to<std::vector<int>>;
      }),
      std::vector<int>(bit_width), [&](const auto &a, const auto &b) {
        return pairwise_add(a, b) | to<std::vector>;
      });

  // Most comon bit is one if the sum of the column is
  // greater than half the column size
  auto most_common_bits =
      bit_counts | views::transform([m = inputs.size() / 2](auto x) -> int {
        return x > m;
      }) |
      to<std::vector>;
  auto gamma = accumulate(most_common_bits, uint64_t(0),
                          [](auto cur, auto n) { return cur << 1 | n; });
  auto eps = ~gamma & mask;
  fmt::print("{}, {} {} {}\n", most_common_bits, gamma, eps, gamma * eps);
  return gamma * eps;
}

int solve_problem_2(const Input &inputs) {
  using namespace ranges;
  auto digit_c_to_i = [](auto d) { return d - '0'; };
  auto most_common_bit_at = [digit_c_to_i](const auto bit_pos,
                                           const auto &vals) -> int {
    return accumulate(vals | views::transform(
                                 [&](const auto &v) { return v[bit_pos]; }),
                      uint64_t(0), std::plus{}, digit_c_to_i) *
               2 >=
           vals.size();
  };
  auto o2_values = inputs;
  for (int cur_bit = 0; o2_values.size() > 1; cur_bit++) {
    auto bit_needed = most_common_bit_at(cur_bit, o2_values);
    o2_values = o2_values |
                views::filter([cur_bit, hunt = '0' + bit_needed](auto v) {
                  return (hunt == v[cur_bit]);
                }) |
                to<std::vector>;
  }

  auto co2_values = inputs;
  for (int cur_bit = 0; co2_values.size() > 1; cur_bit++) {
    auto bit_needed = 1 - most_common_bit_at(cur_bit, co2_values);
    co2_values = co2_values |
                 views::filter([cur_bit, hunt = '0' + bit_needed](auto v) {
                   return (hunt == v[cur_bit]);
                 }) |
                 to<std::vector>;
  }
  auto o2v = accumulate(o2_values.front(), uint64_t(0),
                        [](auto cur, auto n) { return cur << 1 | n - '0'; });
  auto co2v = accumulate(co2_values.front(), uint64_t(0),
                         [](auto cur, auto n) { return cur << 1 | n - '0'; });
  return co2v * o2v;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::print("specify an input file please\n");
    return 1;
  }
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs =
      ranges::istream_range<InputItem>(input_file) | ranges::to<Input>();

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

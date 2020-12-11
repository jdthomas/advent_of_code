#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

using Input = std::vector<uint64_t>;

// Copy pasta from day one problem, converted to iterators because I thought I
// forgot my algorithm needed a sorted window, meh
bool find_sum(const Input &inputs, int search_num) {
  for (auto s = begin(inputs), e = end(inputs) - 1; s != e;) {
    const auto cur = *s + *e;
    // fmt::print("Checking {} + {} = {} ?= {}\n", *s, *e, cur, search_num);
    if (cur < search_num) {
      ++s;
    } else if (cur > search_num) {
      --e;
    } else {
      return true;
      break;
    }
  }
  return false;
}
int solve_problem_1(const Input &inputs) {
  Input tmp_srt;
  const int window_size = 25;
  for (int i = window_size; i < inputs.size(); i++) {
    auto e = begin(inputs) + i;
    auto s = e - window_size;
    tmp_srt.clear();
    std::copy(s, e, back_inserter(tmp_srt));
    actions::sort(tmp_srt);
    if (!find_sum(tmp_srt, inputs[i])) {
      return inputs[i];
    };
  }
  return 42;
}

int solve_problem_2(const Input &inputs) {
  // fmt::print("srt: {}\n", inputs);
  const uint64_t search_num = 70639851;
  // const uint64_t search_num = 127;
  for (auto s = begin(inputs), e = begin(inputs) + 1;
       s != end(inputs) && e != end(inputs);) {
    const auto cur = std::accumulate(s, e, 0ull);
    // fmt::print("Checking sum({}, {}) = {} ?= {}\n",
    //            std::distance(begin(inputs), s), std::distance(begin(inputs),
    //            e), cur, search_num);
    if (cur < search_num) {
      // too small, add another one to end
      ++e;
    } else if (cur > search_num) {
      // too big, drop the first one
      ++s;
    } else {
      auto [a, b] = std::minmax_element(s, e);
      Input tmp;
      std::copy(s, e, std::back_inserter(tmp));
      fmt::print("Found solution {} + {} in {}\n", *a, *b, tmp);
      return *a + *b;
      break;
    }
  }
  return 42;
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;

  std::copy(std::istream_iterator<uint64_t>(input_file),
            std::istream_iterator<uint64_t>(), std::back_inserter(inputs));
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

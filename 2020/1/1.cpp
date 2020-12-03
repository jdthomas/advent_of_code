
// clang  -O3 -std=c++17 -g0 -Wall -Wextra -lstdc++ -lfmt -L$HOME/homebrew/lib
// -I$HOME/homebrew/include/ 1.cpp

#include <fstream>
#include <iostream>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;
#include <fmt/core.h>
#include <fmt/ranges.h>

int main(int argc, char **argv) {
  std::vector<int> inputs;
  std::fstream input_file(argv[1], std::ios_base::in);
  std::copy(std::istream_iterator<int>(input_file),
            std::istream_iterator<int>(), std::back_inserter(inputs));

  actions::sort(inputs);
  fmt::print("{}\n", inputs);

  const int search_num = 2020;

  // TODO: How to represent w/ algorithms

  // Part 1
  for (int i = 0, j = inputs.size() - 1; i < j;) {
    const auto cur = inputs[i] + inputs[j];
    // fmt::print("cur: {} ({},{})\n", cur, i,j);
    if (cur < search_num) {
      ++i;
    } else if (cur > search_num) {
      --j;
    } else {
      fmt::print("Found: {} * {} = {} \n", inputs[i], inputs[j],
                 inputs[i] * inputs[j]);
      break;
    }
  }

  // part 2
  for (auto maybe : inputs) {
    // Copy of part 1, for each entry (should add a check that we don't use
    // maybe 2x, and should bail from main loop instead of continuing search,
    // meh)
    const int search_num2 = 2020 - maybe;
    for (int i = 0, j = inputs.size() - 1; i < j;) {
      const auto cur = inputs[i] + inputs[j];
      if (cur < search_num2) {
        ++i;
      } else if (cur > search_num2) {
        --j;
      } else {
        fmt::print("Found: {} * {} * {} = {} \n", maybe, inputs[i], inputs[j],
                   maybe * inputs[i] * inputs[j]);
        break;
      }
    }
  }
}

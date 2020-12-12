#include <fstream>
#include <iostream>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

using Input = std::vector<std::vector<char>>;

constexpr bool inbounds(const int x, int y, const Input &state) {
  return 0 <= x && x < state.size() && 0 <= y && y < state[x].size();
}

int count_neighbors(const Input &state, int i, int j) {
  int ct = 0;
  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      auto xx = x + i;
      auto yy = y + j;
      if ((x == 0 && y == 0) || !inbounds(xx, yy, state))
        continue;
      // fmt::print("{} {}: {},{} < {} {} \n", xx, yy, i, j, state.size(),
      // state[0].size()); fmt::print("state[x][y] = {}\n", state[xx][yy]);
      ct += state.at(xx).at(yy) == '#';
    }
  }
  return ct;
}

char find_on_slope(const int step_x, const int step_y, int x, int y,
                   const Input &state) {
  x += step_x;
  y += step_y;
  while (inbounds(x, y, state)) {
    if (state[x][y] != '.') {
      return state[x][y];
    }
    x += step_x;
    y += step_y;
  }
  return '.';
}

int count_visible(const Input &state, int i, int j) {
  int ct = 0;
  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      if (x == 0 && y == 0)
        continue;
      ct += find_on_slope(x, y, i, j, state) == '#';
    }
  }
  return ct;
}

void pprint(const std::vector<std::vector<int>> &state) {
  auto tmp = state |
             views::transform([](auto r) { return fmt::format("{}", r); }) |
             to<std::vector<std::string>>();
  fmt::print("{}\n", tmp | views::join("\n") | to<std::string>());
}
void pprint(const Input &state) {
  auto tmp = state |
             views::transform([](auto r) { return r | to<std::string>(); }) |
             to<std::vector<std::string>>();
  fmt::print("{}\n", tmp | views::join("\n") | to<std::string>());
}

int solve_problem_1(const Input &inputs) {
  auto state = inputs;
  int changed = 0;
  int iterations = 0;
  do {
    iterations += 1;
    changed = 0;

    auto next_state = state;
    std::vector<std::vector<int>> neighbor_state(state.size());
    for (size_t i = 0; i < state.size(); i++) {
      for (size_t j = 0; j < state[i].size(); j++) {
        next_state[i][j] = state[i][j];
        // Not a seat
        if (next_state[i][j] == '.') {
          continue;
        }
        auto const neighbors = count_neighbors(state, i, j);
        neighbor_state[i].push_back(neighbors);

        if (state[i][j] == 'L' && neighbors == 0) {
          // If a seat is empty (L) and there are no occupied seats adjacent to
          // it, the seat becomes occupied.
          next_state[i][j] = '#';
          changed++;
        } else if (state[i][j] == '#' && neighbors >= 4) {
          // If a seat is occupied (#) and four or more seats adjacent to it are
          // also occupied, the seat becomes empty
          next_state[i][j] = 'L';
          changed++;
        } else {
        }
      }
    }
    // pprint(state);
    // pprint(neighbor_state);
    state = next_state;
    // for each item in state, check
  } while (changed != 0);
  fmt::print("{} iterations\n", iterations);
  fmt::print("{}\n", state | views::transform([](auto r) {
                       return r | to<std::string>();
                     }) | to<std::vector<std::string>>);
  return accumulate(
      state | views::transform([](auto row) { return count(row, '#'); }), 0);
}

int solve_problem_2(const Input &inputs) {
  auto state = inputs;
  int changed = 0;
  int iterations = 0;
  do {
    iterations += 1;
    changed = 0;
    auto next_state = state;
    std::vector<std::vector<int>> neighbor_state(state.size());
    for (size_t i = 0; i < state.size(); i++) {
      for (size_t j = 0; j < state[i].size(); j++) {
        next_state[i][j] = state[i][j];
        // Not a seat
        if (next_state[i][j] == '.') {
          continue;
        }
        auto const neighbors = count_visible(state, i, j);
        neighbor_state[i].push_back(neighbors);

        if (state[i][j] == 'L' && neighbors == 0) {
          // If a seat is empty (L) and there are no occupied seats adjacent to
          // it, the seat becomes occupied.
          next_state[i][j] = '#';
          changed++;
        } else if (state[i][j] == '#' && neighbors >= 5) {
          // If a seat is occupied (#) and four or more seats adjacent to it are
          // also occupied, the seat becomes empty
          next_state[i][j] = 'L';
          changed++;
        } else {
        }
      }
    }
    // pprint(state);
    // pprint(neighbor_state);
    state = next_state;
    // for each item in state, check
  } while (changed != 0);
  fmt::print("{} iterations\n", iterations);
  fmt::print("{}\n", state | views::transform([](auto r) {
                       return r | to<std::string>();
                     }) | to<std::vector<std::string>>);
  return accumulate(
      state | views::transform([](auto row) { return count(row, '#'); }), 0);
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs = istream_range<std::string>(input_file) | to<Input>();
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

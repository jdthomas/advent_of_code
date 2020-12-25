#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

using InputItem = std::string;
using Input = std::vector<InputItem>;
using Position = std::pair<double, double>;

using Coord3 = std::tuple<int, int, int>;
using Coord4 = std::tuple<int, int, int, int>;
struct coord_hash {
  std::size_t operator()(const Coord3 &coord) const {
    return std::hash<int>()(std::get<0>(coord)) ^
           std::hash<int>()(std::get<1>(coord)) ^
           std::hash<int>()(std::get<2>(coord));
  }
  std::size_t operator()(const Coord4 &coord) const {
    return std::hash<int>()(std::get<0>(coord)) ^
           std::hash<int>()(std::get<1>(coord)) ^
           std::hash<int>()(std::get<2>(coord)) ^
           std::hash<int>()(std::get<3>(coord));
  }
};
using Coord3Set = std::unordered_set<Coord3, coord_hash>;
using Coord4Set = std::unordered_set<Coord4, coord_hash>;

std::vector<Coord3> gen_neighbors(Coord3 coord) {
  std::vector<Coord3> result;
  auto [xx, yy, zz] = coord;
  for (int x = -1; x <= 1; x++)
    for (int y = -1; y <= 1; y++)
      for (int z = -1; z <= 1; z++) {
        if (x == 0 && y == 0 && z == 0)
          continue;
        result.emplace_back(xx + x, yy + y, zz + z);
      }
  return result;
}

std::vector<Coord4> gen_neighbors(Coord4 coord) {
  std::vector<Coord4> result;
  auto [xx, yy, zz, ww] = coord;
  for (int x = -1; x <= 1; x++)
    for (int y = -1; y <= 1; y++)
      for (int z = -1; z <= 1; z++)
        for (int w = -1; w <= 1; w++) {
          if (x == 0 && y == 0 && z == 0 && w == 0)
            continue;
          result.emplace_back(xx + x, yy + y, zz + z, ww + w);
        }
  return result;
}

int solve_problem_1(const Input &inputs) {
  // During a cycle, all cubes simultaneously change their state according to
  // the following rules:
  //   If a cube is active and exactly 2 or 3 of its neighbors are also active,
  //   the cube remains active. Otherwise, the cube becomes inactive. If a cube
  //   is inactive but exactly 3 of its neighbors are active, the cube becomes
  //   active. Otherwise, the cube remains inactive.
  //
  //
  Coord3Set world_state;
  for (int i = 0; i < inputs.size(); i++)
    for (int j = 0; j < inputs[i].size(); j++)
      if (inputs[i][j] == '#')
        world_state.insert(make_tuple(i, j, 1));

  const int rounds = 6;
  for (int i = 0; i < rounds; i++) {
    Coord3Set next_world_state;
    next_world_state.reserve(world_state.size() * 2);
    for (auto coord : world_state) {
      int active_coord_ct = 0;
      // For each neighbor of an active item
      for (auto n : gen_neighbors(coord)) {
        bool n_active = find(world_state, n) != end(world_state);
        active_coord_ct += n_active;
        if (n_active || find(next_world_state, n) != end(next_world_state))
          continue; // will check as a coord, or already checked here
        // count the number of items on
        int active_n_ct = 0;
        for (auto n : gen_neighbors(n)) {
          active_n_ct += find(world_state, n) != end(world_state);
        }

        // becomes active
        if (active_n_ct == 3)
          next_world_state.insert(n);
      }
      // Stay active
      if (active_coord_ct == 2 || active_coord_ct == 3)
        next_world_state.insert(coord);
    }
    fmt::print("{}\n", world_state);
    fmt::print("World State after {} rounds has {} active\n", i,
               world_state.size());
    world_state = next_world_state;
  }
  fmt::print("World State after {} rounds has {} active\n", rounds,
             world_state.size());
  return world_state.size();
}

int solve_problem_2(const Input &inputs) {
  // During a cycle, all cubes simultaneously change their state according to
  // the following rules:
  //   If a cube is active and exactly 2 or 3 of its neighbors are also active,
  //   the cube remains active. Otherwise, the cube becomes inactive. If a cube
  //   is inactive but exactly 3 of its neighbors are active, the cube becomes
  //   active. Otherwise, the cube remains inactive.
  //
  //
  Coord4Set world_state;
  for (int i = 0; i < inputs.size(); i++)
    for (int j = 0; j < inputs[i].size(); j++)
      if (inputs[i][j] == '#')
        world_state.insert(make_tuple(i, j, 1, 1));

  const int rounds = 6;
  for (int i = 0; i < rounds; i++) {
    Coord4Set next_world_state;
    for (auto coord : world_state) {
      int active_coord_ct = 0;
      // For each neighbor of an active item
      for (auto n : gen_neighbors(coord)) {
        bool n_active = find(world_state, n) != end(world_state);
        active_coord_ct += n_active;
        if (n_active)
          continue; // will check as a coord
        // count the number of items on
        int active_n_ct = 0;
        for (auto n : gen_neighbors(n)) {
          active_n_ct += find(world_state, n) != end(world_state);
        }

        // becomes active
        if (active_n_ct == 3)
          next_world_state.insert(n);
      }
      // Stay active
      if (active_coord_ct == 2 || active_coord_ct == 3)
        next_world_state.insert(coord);
    }
    fmt::print("{}\n", world_state);
    fmt::print("World State after {} rounds has {} active\n", i,
               world_state.size());
    world_state = next_world_state;
  }
  fmt::print("World State after {} rounds has {} active\n", rounds,
             world_state.size());
  return world_state.size();
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

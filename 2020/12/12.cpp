#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

using InputItem = std::pair<char, int>;
using Input = std::vector<InputItem>;
using Position = std::pair<double, double>;

constexpr long double deg2rad(double deg) { return deg * (M_PI / 180.); }
constexpr long double rad2deg(double rad) { return rad * 180.0 / M_PI; }
Position update_position(const Position &pos, int direction, int distance) {

  return std::make_pair(
      std::round(pos.first + std::cos(deg2rad(direction)) * distance),
      std::round(pos.second + std::sin(deg2rad(direction)) * distance));
}

int heading_from_compass(char compass_direction) {
  // Defining N as 0 degrees;
  switch (compass_direction) {
  case 'N':
    return 0;
  case 'W':
    return 90;
  case 'S':
    return 180;
  case 'E':
    return 270;
  }
  throw std::runtime_error("whoo");
}
int solve_problem_1(const Input &inputs) {
  // Something borked with my coordinate system, but it works, East / West are
  // flipped, whoops?
  auto cur_heading = heading_from_compass('E');
  Position cur_position{0, 0};
  for (auto [action, distance] : inputs) {
    fmt::print("heading {} at {}, moving {} {}\n", cur_heading, cur_position,
               action, distance);

    switch (action) {
    case 'R':
      cur_heading = (360 + cur_heading - distance) % 360;
      break;
    case 'L':
      cur_heading = (360 + cur_heading + distance) % 360;
      break;
    case 'F':
      cur_position = update_position(cur_position, cur_heading, distance);
      break;
    case 'N':
    case 'E':
    case 'S':
    case 'W':
      cur_position =
          update_position(cur_position, heading_from_compass(action), distance);
      break;
    }
  }
  fmt::print("Final pos: {} final heading {}\n", cur_position, cur_heading);
  return std::abs(cur_position.first) + std::abs(cur_position.second);
}

void pprint(const Position &pos, std::string name = "") {
  fmt::print("{}: {} units {} and {} units {}\n", name, std::abs(pos.first),
             pos.first > 0 ? "north" : "south", std::abs(pos.second),
             pos.second > 0 ? "west" : "east");
}

uint64_t solve_problem_2(const Input &inputs) {
  // E/W, N/S coordinates
  Position waypoint{1, 10};
  Position cur_position{0, 0};
  for (auto [action, distance] : inputs) {
    fmt::print("Waypoint {} position {} action {} distance {}\n", waypoint,
               cur_position, action, distance);
    switch (action) {
    case 'L':
      distance = 360 - distance;
    case 'R': {
      auto heading_rad = deg2rad(distance);
      auto heading = rad2deg(heading_rad);
      pprint(waypoint, "waypoint");
      pprint(cur_position, "position");
      fmt::print("heading to waypoint: {} (Rad:{}) s: {} c: {}\n", heading,
                 heading_rad, std::sin(heading_rad), std::cos(heading_rad));
      auto tmp = waypoint;
      waypoint =
          std::make_pair(std::round(std::cos(heading_rad) * waypoint.first -
                                    std::sin(heading_rad) * waypoint.second),
                         std::round(std::sin(heading_rad) * waypoint.first +
                                    std::cos(heading_rad) * waypoint.second));
      // assert that we didn't miss since all turns are multiples of 90
      assert(std::abs(waypoint.first) == std::abs(tmp.first) ||
             std::abs(waypoint.first) == std::abs(tmp.second));
      assert(std::abs(waypoint.second) == std::abs(tmp.first) ||
             std::abs(waypoint.second) == std::abs(tmp.second));
    } break;
    case 'F': {
      cur_position =
          std::make_pair(waypoint.first * distance + cur_position.first,
                         waypoint.second * distance + cur_position.second);
    } break;
    case 'N':
    case 'E':
    case 'S':
    case 'W':
      waypoint =
          update_position(waypoint, heading_from_compass(action), distance);
      break;
    }
  }
  fmt::print("Final waypoint {} position {} \n", waypoint, cur_position);
  return std::abs(cur_position.first) + std::abs(cur_position.second);
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs =
      istream_range<std::string>(input_file) | views::transform([](auto entry) {
        auto action = entry | views::take(1) | to<std::string>;
        int distance = std::stoi(entry | views::drop(1) | to<std::string>);
        return std::make_pair(action[0], distance);
      }) |
      to<Input>();

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

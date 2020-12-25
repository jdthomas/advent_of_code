#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

using Input = std::vector<std::vector<std::string>>;

const std::array<std::string, 7> required_fields{{
    "byr", // (Birth Year)
           // "cid", // (Country ID)
    "ecl", // (Eye Color)
    "eyr", // (Expiration Year)
    "hcl", // (Hair Color)
    "hgt", // (Height)
    "iyr", // (Issue Year)
    "pid", // (Passport ID)
}};

// auto dummy(std::string) -> bool;
// using Validator = decltype(dummy);
using Validator = bool (*)(std::string);

const std::unordered_map<std::string, Validator> validators{
    {"byr",
     [](std::string val) {
       //(Birth Year) - four digits; at least 1920 and at most 2002.
       int yr = std::stoi(val);
       return yr >= 1920 && yr <= 2002;
     }},
    {"iyr",
     [](std::string val) {
       //(Issue Year) - four digits; at least 2010 and at most 2020.
       int yr = std::stoi(val);
       return yr >= 2010 && yr <= 2020;
     }},
    {"eyr",
     [](std::string val) {
       //(Expiration Year) - four digits; at least 2020 and at most 2030.
       int yr = std::stoi(val);
       return yr >= 2020 && yr <= 2030;
     }},
    {"hgt",
     [](std::string val) {
       //(Height) - a number followed by either cm or in: If cm, the number
       // must be at least 150 and at most 193.  If in, the number must be at
       // least 59 and at most 76.
       int ht = std::stoi(val);
       return (val.find("cm") != std::string::npos) ? (150 <= ht && ht <= 193)
                                                    : (59 <= ht && ht <= 76);
     }},
    {"hcl",
     [](std::string val) {
       //(Hair Color) - a # followed by exactly six characters 0-9 or a-f.
       return val.size() == 7 && val[0] == '#' &&
              (val | views::drop(1) |
               views::transform([](auto x) { return std::isxdigit(x); }) |
               views::all);
     }},
    {"ecl",
     [](std::string val) {
       //(Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
       static const std::array<std::string, 7> valid_eyes{
           {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"}};
       return std::find(begin(valid_eyes), end(valid_eyes), val) !=
              end(valid_eyes);
     }},
    {"pid",
     [](std::string val) {
       //(Passport ID) - a nine-digit number, including leading zeroes.
       return val.size() == 9 &&
              (val | views::transform([](auto x) { return std::isdigit(x); }) |
               views::all);
     }},
    {"cid", [](std::string val) {
       //(Country ID) - ignored, missing or not.
       // return false so we just ignore this when counting valid fields, yuk
       return false;
     }}};

int solve_problem_1(const Input &inputs) {
  return accumulate(
      inputs | views::transform([](auto fields) {
        auto ct = accumulate(
            fields | views::transform([](auto field_name) {
              return find(required_fields,
                          field_name | views::take_while([](auto c) {
                            return c != ':';
                          }) | to<std::string>) != end(required_fields)
                         ? 1
                         : 0;
            }),
            0);
        // fmt::print("{} has {} fields\n", fields, ct);
        return ct == required_fields.size() ? 1 : 0;
      }),
      0);
}

int solve_problem_2(const Input &inputs) {
  return accumulate(
      inputs | views::transform([](auto fields) {
        auto ct = accumulate(
            fields | views::transform([](auto field_str) {
              auto field_name =
                  field_str |
                  views::take_while([](auto c) { return c != ':'; }) |
                  to<std::string>;
              auto field_val =
                  field_str |
                  views::drop_while([](auto c) { return c != ':'; }) |
                  views::drop(1) | to<std::string>;
              auto v = validators.find(field_name);
              return v != validators.end() && v->second(field_val);
            }) | views::all,
            0);
        // fmt::print("{} has {} fields\n", fields, ct);
        return ct == required_fields.size() ? 1 : 0;
      }),
      0);
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;

  // Lazy JT
  // std::vector<std::string>
  // lines{std::istream_iterator<std::string>(input_file),
  // std::istream_iterator<std::string>()};
  std::vector<std::string> current;
  std::string line;
  while (getline(input_file, line)) {
    fmt::print("line: {}", line);
    if (line.empty()) {
      // empty lines delimante entries
      inputs.push_back(current);
      current.clear();
    } else {
      auto pairs = line | views::split(' ') | to<std::vector<std::string>>;
      std::copy(begin(pairs), end(pairs), std::back_inserter(current));
    }
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

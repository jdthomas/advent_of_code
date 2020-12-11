#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

enum Command {
  NOP,
  ACC,
  JMP,
};

std::pair<Command, int> command_parse(const std::string &s) {
  static const std::unordered_map<std::string, Command> lookup{
      {"nop", Command::NOP}, {"acc", Command::ACC}, {"jmp", Command::JMP}};
  auto c = lookup.find(s | views::take(3) | to<std::string>);
  fmt::print("{}\n", s | views::drop(4) | to<std::string>);
  auto v = std::stoi(s | views::drop(4) | to<std::string>);
  if (c == end(lookup)) {
    return {Command::NOP, 0};
  }
  return std::make_pair(c->second, v);
}

using Input = std::vector<std::string>;

std::pair<int, bool> run_program(const Input &inputs) {
  int pc = 0, acc = 0;
  std::vector<bool> ran(inputs.size());
  while (true) {
    if (pc == inputs.size()) {
      return {acc, true};
    }
    if (pc >= inputs.size() || pc < 0) {
      return {acc, false};
    }
    auto line = inputs[pc];
    fmt::print("{}\n", line);
    fmt::print("\tpc={} acc={}\n", pc, acc);
    if (ran[pc]) {
      fmt::print("Attempting to re-run command at pc={}\n", pc);
      return {acc, false};
    }
    ran[pc] = true;

    auto [command, value] = command_parse(line);
    switch (command) {
    case Command::NOP:
      pc += 1;
      break;
    case Command::ACC:
      acc += value;
      pc += 1;
      break;
    case Command::JMP:
      pc += value;
      break;
    }
  }
  return {42, false};
}

int solve_problem_1(const Input &inputs) { return run_program(inputs).first; }

int solve_problem_2(Input inputs) {
  const std::string jmp{"jmp"}, nop{"nop"};
  for (auto &line : inputs) {
    std::string backup = line;
    // Manipulate program
    auto [command, value] = command_parse(line);
    switch (command) {
    case Command::ACC:
      continue;
    case Command::NOP:
      std::copy(begin(jmp), end(jmp), begin(line));
      break;
    case Command::JMP:
      std::copy(begin(nop), end(nop), begin(line));
      break;
    }
    // run program
    auto [acc, success] = run_program(inputs);
    if (success)
      return acc;

    // restore program
    line = backup;
  }
  return -41;
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;

  std::string line;
  while (getline(input_file, line)) {
    if (line.empty()) {
    } else {
    }
    inputs.push_back(line);
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

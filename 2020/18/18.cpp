#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

using InputItem = std::string;
using Input = std::vector<InputItem>;
using Position = std::pair<double, double>;

class Operation {
public:
  Operation() = default;
  virtual ~Operation() = default;
};
class Plus : public Operation {
  Operation *left, *right;
};
class Mult : public Operation {
  Operation *left, *right;
};
class Literal : public Operation {
public:
  Literal() = default;
  int value;
};
using Op = std::variant<Literal, Mult, Plus>;

int solve_problem_1(const Input &inputs) {
  std::string input = "4 * (7 + 5 + 7) + 9 + 2 * ((5 * 6) * 9 + 6 + 3) + 3";
  std::vector<Operation> ops;
  // for(auto i: inputs) {
  int accum = 0;
  std::unique_ptr<Operation> cur_op = nullptr;
  for (auto ch : input) {
    if (std::isdigit(ch)) {
      if (cur_op == nullptr) {
        cur_op = std::unique_ptr<Operation>(new Literal());
      }
      Literal *lit_op = dynamic_cast<Literal *>(cur_op.get());
      if (lit_op) {
        lit_op->value = lit_op->value * 10 + int(ch) - int('a');
        fmt::print("val: {}\n", lit_op->value);
      }
    } else if (std::isspace(ch)) {
      push_back(*cur_op);
      cur_op = nullptr;
      continue;
    } else if (ch == '(') {
    } else if (ch == ')') {
    } else if (ch == '+') {
    } else if (ch == '*') {
    }
  }
  //}
  return 42;
}

int solve_problem_2(const Input &inputs) { return 42; }

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

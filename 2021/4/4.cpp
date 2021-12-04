#include <fmt/core.h>
#include <fmt/ranges.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
#include <variant>
#include <vector>
using namespace ranges;

constexpr uint32_t board_size = 5;
using Board = std::array<int64_t, board_size * board_size>;

struct Input {
  std::vector<int64_t> draw;
  std::vector<Board> boards;
};

bool check_winning_board(const Board &values, const Board &marked) {
  auto col_ct = [&](const auto &col) {
    return accumulate(marked | views::drop(col) | views::stride(board_size) |
                          views::take(board_size),
                      0);
  };
  auto row_ct = [&](const auto &row) {
    return accumulate(
        marked | views::drop(board_size * row) | views::take(board_size), 0);
  };
  auto x = views::iota(0, 5);
  return x.end() != find_if(x, [&](const auto &idx) {
           // square board, can check both with same index
           return col_ct(idx) == board_size || row_ct(idx) == board_size;
         });
}

auto board_score = [](const auto &board, const auto &marked_board) {
  // returns the sum of all unmarked cells on the board
  return inner_product(marked_board, board, 0, std::plus{},
                       [](const auto &marked, const auto &value) {
                         return 0 == marked ? value : 0;
                       });
};

auto mark_called_cells = [](auto &board, auto &marked_board, int64_t call) {
  for (const auto &p : views::zip(board, marked_board)) {
    auto &[v, m] = p;
    m = (v == call) ? 1 : m;
  }
};

int64_t solve_problem_1(const Input &inputs) {
  std::vector<Board> marked_boards(inputs.boards.size());
  for (int call : inputs.draw) {
    auto score = std::inner_product(
        inputs.boards.begin(), inputs.boards.end(), marked_boards.begin(),
        int64_t(0),
        // TODO: inner_product not short circuiting, is better choice?
        // just collect first winner.
        [](const int64_t acc, const int64_t score) {
          return acc ? acc : score;
        },
        [call](auto &board, auto &marked) -> int64_t {
          mark_called_cells(board, marked, call);

          return check_winning_board(board, marked) ? board_score(board, marked)
                                                    : 0;
        });
    if (score != 0) {
      return call * score;
    }
  }
  return 42;
}

int solve_problem_2(Input &inputs) {
  std::vector<Board> marked_boards(inputs.boards.size());
  int winning_boards = 0;
  for (int call : inputs.draw) {
    std::vector<int64_t> scores;
    std::transform(
        inputs.boards.begin(), inputs.boards.end(),
        marked_boards.begin() /*, marked_boards.end()*/, back_inserter(scores),
        [call](auto &board, auto &marked) -> int64_t {
          mark_called_cells(board, marked, call);

          return check_winning_board(board, marked) ? board_score(board, marked)
                                                    : -1;
        });
    // Last board just won, return score
    if (marked_boards.size() == 1 && scores.front() != -1) {
      return call * scores.front();
    }
    // Yuk, remove solved boards
    auto z = views::zip(inputs.boards, marked_boards, scores);
    auto e = remove_if(z, [](const auto &p) {
      const auto &[b, m, s] = p;
      return s != -1;
    });
    auto offset = std::distance(z.begin(), e);
    inputs.boards.erase(inputs.boards.begin() + offset, inputs.boards.end());
    marked_boards.erase(marked_boards.begin() + offset, marked_boards.end());
  }
  return 42;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::print("specify an input file please\n");
    return 1;
  }
  std::ifstream input_file(argv[1], std::ios_base::in);
  // auto lines = istream_range<std::string>(input_file) |
  //              to<std::vector<std::string>>();
  // fmt::print("{}\n", lines);

  // comma seperated ints
  std::string line;
  getline(input_file, line);
  Input inputs;
  inputs.draw = line | views::split(',') | views::transform([](auto i) {
                  return int64_t(stoll(i | to<std::string>));
                }) |
                to<std::vector>;
  fmt::print("{}\n", inputs.draw);
  while (getline(input_file, line)) {
    // empty line
    std::vector<int64_t> v;
    for (int row = 0; row < 5; row++) {
      getline(input_file, line);
      std::stringstream ugh{line};
      int64_t a;
      for (int col = 0; col < 5; col++) {
        ugh >> a;
        v.push_back(a);
      }
    }
    fmt::print("{}\n", v);
    inputs.boards.emplace_back();
    std::copy(v.begin(), v.end(), inputs.boards.back().begin());
    // auto row = line | views::split(' ') |
    //            views::transform([](auto i) {
    //                                     fmt::print("i={}\n", i |
    //                                     to<std::string>);
    //              return int64_t(stoll(i | to<std::string>));
    //            }) |
    //            to<std::vector>;
    //   Board b{};
    //   for(int i: views::iota(board_size)) {
    //     // copy 5 integers from stream to new board
    //   }
  }

  // fmt::print("{}\n", inputs);

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

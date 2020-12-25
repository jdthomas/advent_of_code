#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

struct Bag {
  using Content = std::vector<std::pair<int, std::string>>;
  Bag(std::string &n) : name(n){};
  Bag(std::string &&n, Content &&c) : name(n), contents(c){};
  std::string name;
  Content contents;
};
template <> struct fmt::formatter<Bag> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(Bag const &bag, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "{0}: {1}", bag.name, bag.contents);
  }
};

using Input = std::vector<Bag>;

int solve_problem_1(const Input &inputs) {
  std::unordered_set<std::string> checked;
  std::vector<std::string> searching;
  searching.push_back("shinygold");

#if 0
  // Oops, this is what bags can be containd in a shiny gold, got that backwards :(
  while (searching.size()) {
    auto s = searching.back();
    fmt::print("can be in bag: {}\n", s);
    searching.pop_back();
    if (std::find(begin(checked), end(checked), s) != end(checked)) {
      continue;
    }
    checked.insert(s);
    auto cur_bag =
        find_if(inputs, [name = s](auto b) { return name == b.name; });
    if (cur_bag == end(inputs)) {
      fmt::print("That's unfortunate, couldnt fund the current bag: {}\n", s);
      continue;
    }
    auto new_bags = cur_bag->contents | views::transform([](auto c) {
                      auto [ct, name] = c;
                      return name;
                    }) |
                    to<std::vector<std::string>>;

    fmt::print("Adding more: {}\n", new_bags);
    std::copy(begin(new_bags), end(new_bags), std::back_inserter(searching));
  }
#else

  // Make the reverse mapping?
  std::unordered_map<std::string, std::vector<std::string>> r_contains;
  for (auto x : inputs) {
    auto [k, v] = x;
    for (auto name : v) {
      r_contains[name.second].push_back(k);
    }
  }

  while (searching.size()) {
    auto s = searching.back();
    fmt::print("can be in bag: {}\n", s);
    searching.pop_back();
    if (std::find(begin(checked), end(checked), s) != end(checked)) {
      continue;
    }
    checked.insert(s);

    auto new_bags = r_contains[s];

    fmt::print("Adding more: {}\n", new_bags);
    std::copy(begin(new_bags), end(new_bags), std::back_inserter(searching));
  }
#endif
  fmt::print("{}\n", checked);
  return checked.size() -
         1 /*minus one because I stuck the search query in checked*/;
}

int bags_of_bags(const std::string &name, const Input &inputs) {
  const auto &cur_bag =
      find_if(inputs, [name](auto b) { return name == b.name; });
  if (cur_bag == end(inputs)) {
    return 0;
  }
  auto ct = accumulate(cur_bag->contents | views::transform([inputs](auto c) {
                         auto [ct, name] = c;
                         return ct * bags_of_bags(name, inputs);
                       }),
                       1);
  fmt::print("cur bag: {}, ct: {}\n", *cur_bag, ct);
  return ct;
}

int solve_problem_2(const Input &inputs) {
  std::vector<std::string> searching;
  std::string start = "shinygold";
  return bags_of_bags(start, inputs) - 1;
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  Input inputs;

  // [, a-zA-Z0-9]
  auto const r = std::regex("([a-zA-Z ]*) bags contain (.*)\\.");

  std::string line;
  while (getline(input_file, line)) {
    std::smatch match;
    std::regex_match(line, match, r);
    fmt::print("line: '{}' matches {} things\n", line, match.size());
    if (match.size() >= 3) {
      auto content_str = match[2].str();
      auto cur_bag = inputs.emplace_back(
          std::string_view(match[1].str()) |
              views::filter([](auto c) { return c != ' '; }) | to<std::string>,

          content_str | views::split(',') | views::filter([](auto s) {
            return (s | to<std::string>) != "no other bags";
          }) |
              // I'm lazy, just drop all spaces so that keys are teh same.
              views::transform([](auto s) {
                return s | views::filter([](auto c) { return c != ' '; });
              }) |
              views::transform([](auto s) {
                size_t idx;
                int ct = std::stoi(s | to<std::string>, &idx);
#pragma unused(ct)
                return make_pair(ct, s | views::drop(idx) |
                                         views::drop_last(ct > 1 ? 4 : 3) |
                                         to<std::string>);
              }) |
              to<Bag::Content>);
      fmt::print("match[0]: {} match[1]: {} match[2]: {}\n", match[0].str(),
                 cur_bag.name, cur_bag.contents);
    }
  }
  fmt::print("{}\n", inputs.size());

  // This basically looks like a graph problem, we should parse out the bag
  // names and what they can hold into some sort of bag struct:
  // struct bag { string name; vector<int,bag&> contains; };
  // Then we can just walk the tree to collect what is holding what. For the
  // first part of the question we actually want the reverse, bag { name ;
  // contained_by; }, but we can probably just do a BFS search from each
  // staring point to solve that. Expect we will need the contains/count for
  // part 2.
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

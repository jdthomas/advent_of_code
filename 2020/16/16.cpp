#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <vector>

#include <range/v3/action/sort.hpp>
#include <range/v3/all.hpp>
using namespace ranges;

#include <fmt/core.h>
#include <fmt/ranges.h>

struct Field {
  Field(const std::string &nm, int64_t mn1, int64_t mx1, int64_t mn2,
        int64_t mx2)
      : name(nm), min1(mn1), max1(mx1), min2(mn2), max2(mx2){};
  std::string name;
  int64_t min1, max1, min2, max2;
  bool in_range(int64_t q) const {
    return (min1 <= q && q <= max1) || (min2 <= q && q <= max2);
  }
};
template <> struct fmt::formatter<Field> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(Field const &field, FormatContext &ctx) {
    return fmt::format_to(ctx.out(), "Field {0}: {1}-{2} or {3}-{4}",
                          field.name, field.min1, field.max1, field.min2,
                          field.max2);
  }
};

int64_t
solve_problem_1(const std::vector<Field> &fields,
                const std::vector<int64_t> &my_ticket,
                const std::vector<std::vector<int64_t>> &other_tickets_) {
  std::vector<std::vector<int64_t>> other_tickets = other_tickets_;
  // all tickets must have any field that matches each value
  auto bad_ticket = [&](const std::vector<int64_t> &ticket) -> bool {
    return all_of(ticket, [&](const int64_t val) {
      return any_of(fields,
                    [&](const Field &field) { return field.in_range(val); });
    });
  };
  // Filter completely invalid tickets
  actions::remove_if(other_tickets, bad_ticket);

  auto scanning_error = accumulate(
      other_tickets | views::transform([&](auto ticket) {
        return accumulate(ticket | views::transform([&](const auto val) {
                            return any_of(fields,
                                          [&](const Field &field) {
                                            return field.in_range(val);
                                          })
                                       ? 0
                                       : val;
                          }),
                          0);
      }),
      0);

  return scanning_error;
}

int64_t solve_problem_2(const std::vector<Field> &fields,
                        const std::vector<int64_t> &my_ticket,
                        std::vector<std::vector<int64_t>> &other_tickets_) {
  std::vector<std::vector<int64_t>> other_tickets = other_tickets_;
  fmt::print("Starting tickets: {}\n", other_tickets);
  // all tickets must have any field that matches each value
  auto bad_ticket = [&](const std::vector<int64_t> &ticket) -> bool {
    auto ao = !all_of(ticket, [&](const int64_t val) {
      auto ao = any_of(fields,
                       [&](const Field &field) { return field.in_range(val); });
      fmt::print("val: {} {}\n", val, ao);
      return ao;
    });
    fmt::print("tix: {} {}\n", ticket, ao);
    return ao;
  };
  // Filter completely invalid tickets
  actions::remove_if(other_tickets, bad_ticket);
  fmt::print("Cleaned tickets: {}\n", other_tickets);

  // Add my own ticket to the list
  other_tickets.push_back(my_ticket);
  fmt::print("With My ticket: {}\n", other_tickets);

#if 1
  // build a matrix of possible fields for reach position, iterate
  // tickets and remove any out of range ones.
  std::vector<std::vector<int64_t>> valid_fields(fields.size());
  for (auto &v : valid_fields) {
    v = views::indices | views::take(fields.size()) | to<std::vector<int64_t>>;
  }
  fmt::print("HERE: {}\n", valid_fields);
  for (auto ticket : other_tickets) {
    for (auto [val, idx] : views::zip(ticket, views::indices)) {
      fmt::print("Updating valid_fields[{}]=({}) for val {}\n", idx,
                 valid_fields[idx] |
                     views::transform([&](auto i) { return fields[i]; }),
                 val);
      int64_t vv = val;
      actions::remove_if(valid_fields[idx], [&fields, &vv](auto fid) {
        auto a = !fields[fid].in_range(vv);
        fmt::print("maybe remove {} because val {}, {}\n", fid, vv, a);
        return a;
      });
    }
    fmt::print("potentially still valid fields: {}\n", valid_fields);
  }
  // Solve by finding any single entry possiblities and removing that fieldid
  // from all others
  auto changed = true;
  do {
    auto singles =
        valid_fields |
        views::transform([](auto vf) { return vf.size() == 1 ? vf[0] : -1; }) |
        views::filter([](auto v) { return v >= 0; }) | to<std::vector>;
    fmt::print("singles: {}\n", singles);
    valid_fields = valid_fields | views::transform([&singles](auto vf) {
                     auto gz = (vf.size() > 1);
                     return vf | views::filter([&singles, gz](auto v) {
                              auto a = !gz || find(singles, v) == end(singles);
                              fmt::print("== filt: {} {}\n", v, a);
                              return a;
                            }) |
                            to<std::vector>;
                   }) |
                   to<std::vector>;
    changed = singles.size() != fields.size();
  } while (changed);
  fmt::print("still valid fields: {}\n", valid_fields);
  for (auto vf : valid_fields) {
    fmt::print("{}\n", vf | views::transform([&](auto i) {
                         return make_tuple(fields[i].name, my_ticket[i],
                                           strstr(fields[i].name.c_str(),
                                                  "departure") != nullptr);
                       }) | to<std::vector>);
  }
  return accumulate(valid_fields | views::filter([&](auto iv) {
                      auto i = iv[0];
                      return strstr(fields[i].name.c_str(), "departure") !=
                             nullptr;
                    }) | views::transform([&](auto iv) {
                      auto i = iv[0];
                      return my_ticket[i];
                    }),
                    1ull, std::multiplies());
#endif
  return 42;
}

int main(int argc, char **argv) {
  std::ifstream input_file(argv[1], std::ios_base::in);
  std::string line;

  // fields
  std::vector<Field> fields;
  auto const field_matcher =
      std::regex("([a-z ]*): ([0-9]*)-([0-9]*) or ([0-9]*)-([0-9]*)");
  while (getline(input_file, line)) {
    if (line.empty())
      break;
    std::smatch match;
    std::regex_match(line, match, field_matcher);
    fmt::print("Matching '{}' got {}\n", line, match.size());
    if (match.size() >= 6) {
      auto name = match[1].str();
      auto mn1 = std::stoi(match[2].str());
      auto mx1 = std::stoi(match[3].str());
      auto mn2 = std::stoi(match[4].str());
      auto mx2 = std::stoi(match[5].str());
      fields.emplace_back(name, mn1, mx1, mn2, mx2);
    }
  }
  fmt::print("Fields: {}\n", fields);

  // Skip your ticket line
  getline(input_file, line);
  if (line != "your ticket:") {
    fmt::print("invalid input '{}', expected 'your ticket:'\n", line);
    return -1;
  }
  // your ticket
  std::vector<int64_t> my_ticket;
  while (getline(input_file, line)) {
    fmt::print("line: {}\n", line);
    if (line.empty())
      break;
    my_ticket = line | views::split(',') | views::transform([](auto i) {
                  return int64_t(stoll(i | to<std::string>));
                }) |
                to<std::vector>;
  }
  fmt::print("my_ticket: {}\n", my_ticket);

  getline(input_file, line);
  if (line != "nearby tickets:") {
    fmt::print("invalid input '{}', expected 'nearby tickets:'\n", line);
    return -1;
  }

  // nearby
  std::vector<std::vector<int64_t>> other_tickets;
  while (getline(input_file, line)) {
    other_tickets.emplace_back(line | views::split(',') |
                               views::transform([](auto i) {
                                 return int64_t(stoll(i | to<std::string>));
                               }) |
                               to<std::vector>);
  }

  fmt::print("{}\n", other_tickets);

  {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto answer = solve_problem_1(fields, my_ticket, other_tickets);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    fmt::print("Result = {}, resolved in {}usec\n", answer, duration);
  }

  {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto answer = solve_problem_2(fields, my_ticket, other_tickets);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    fmt::print("Result = {}, resolved in {}usec\n", answer, duration);
  }
}

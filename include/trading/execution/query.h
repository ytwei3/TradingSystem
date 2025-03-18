#ifndef TRADING_EXECUTION_QUERY_H
#define TRADING_EXECUTION_QUERY_H

#include <set>
#include <string>

namespace trading {

struct Query {
  long long start;
  long long end;

  std::set<std::string> symbols;
  std::set<int> bid_levels;
  std::set<int> ask_levels;
  std::set<std::string> output_fields;
};

} // namespace trading

#endif // TRADING_EXECUTION_QUERY_H
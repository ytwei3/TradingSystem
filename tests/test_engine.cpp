#include <cassert>
#include <iostream>

#include "trading/execution/engine.hpp"
#include "trading/processor/processor.hpp"

int main() {
  trading::OrderBookProcessor processor;
  processor.ReadMarketData("../data/general_test_data.log");

  std::vector<std::string> symbol{"APPL"};

  processor.ProcessMarketData(symbol[0], 6);

  auto engine = trading::ExecutionEngine();

  auto result = engine.queryOrderBookSnapshots(
      processor, symbol, 1, 3, {3, 2, 1}, {1, 2, 3}, true, true);

  std::cout << result << std::endl;

  return 0;
}
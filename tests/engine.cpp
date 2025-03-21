#include <cassert>
#include <iostream>

#include "gtest/gtest.h"

#include "trading/execution/engine.h"
#include "trading/processor/processor.h"

TEST(engine, reading_data) {
  trading::OrderBookProcessor processor;
  processor.ReadMarketData("../data/general_test_data.log");

  std::vector<std::string> symbol{"APPL"};

  processor.ProcessMarketData(symbol[0], 6);

  auto engine = trading::ExecutionEngine();

  auto result = engine.queryOrderBookSnapshots(
      processor, symbol, 1, 3, {3, 2, 1}, {1, 2, 3}, true, true);

  std::cout << result << std::endl;
}

#ifndef TRADING_EXECUTION_ENGINE_H
#define TRADING_EXECUTION_ENGINE_H

#include <string>
#include <vector>

#include "trading/processor/processor.h"

namespace trading {

class ExecutionEngine {
public:
  ExecutionEngine() = default;
  ~ExecutionEngine() = default;

  std::string queryOrderBookSnapshots(OrderBookProcessor &processor,
                                      std::vector<std::string> symbol,
                                      long long start, long long end,
                                      std::vector<int> bid_levels,
                                      std::vector<int> ask_levels,
                                      bool output_last_trade_price,
                                      bool output_last_trade_quantity) const;
};
} // namespace trading

#endif // TRADING_EXECUTION_ENGINE_H

#include "trading/execution/engine.h"

#include <iomanip>
#include <sstream>

namespace trading {

std::string doubleToString(double num) {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(3) << num;
  return stream.str();
}

std::string ExecutionEngine::queryOrderBookSnapshots(
    OrderBookProcessor &processor, std::vector<std::string> symbols,
    long long start, long long end, std::vector<int> bid_levels,
    std::vector<int> ask_levels, bool output_last_trade_price,
    bool output_last_trade_quantity) const {

  auto result = std::string("symbol, epoch");

  if (!ask_levels.empty() || !bid_levels.empty()) {
    result += ",";
  }

  for (const auto &level : bid_levels) {
    result +=
        " bid" + std::to_string(level) + "q@bid" + std::to_string(level) + "p";
  }

  result += " X";

  for (const auto &level : ask_levels) {
    result +=
        " ask" + std::to_string(level) + "q@ask" + std::to_string(level) + "p";
  }

  if (output_last_trade_price) {
    result += ", last trade price";
  }
  if (output_last_trade_quantity) {
    result += ", last trade quantity";
  }
  result += "\n";

  // check if the snapshot is within the range
  // if no, process up to the end of the range
  for (const auto &symbol : symbols) {
    if (processor.processed_timestamp_ < static_cast<unsigned long long>(end)) {
      processor.ProcessMarketData(symbol, end);
    }

    for (const auto &snap : processor.snapshot_cache_.at(symbol)) {
      // could be optimized by using binary search
      if (snap.epoch >= static_cast<unsigned long long>(start) &&
          snap.epoch <= static_cast<unsigned long long>(end)) {
        result += snap.symbol + ", " + std::to_string(snap.epoch);

        if (!ask_levels.empty() || !bid_levels.empty()) {
          result += ",";
        }

        for (auto i : bid_levels) {
          result += " " +
                    std::to_string(snap.bid_price_levels[i - 1].quantity) +
                    "@" + doubleToString(snap.bid_price_levels[i - 1].price);
        }

        result += " X";

        for (std::size_t i = 0; i < ask_levels.size(); ++i) {
          " " + std::to_string(snap.ask_price_levels[i].quantity) + "@" +
              doubleToString(snap.ask_price_levels[i].price);
        }

        if (output_last_trade_price) {
          if (snap.last_trade_price == 0) {
            result += ", NA";
          } else {
            result += ", " + std::to_string(snap.last_trade_price);
          }
        }
        if (output_last_trade_quantity) {
          if (snap.last_trade_quantity == 0) {
            result += ", NA";
          } else {
            result += ", " + std::to_string(snap.last_trade_quantity);
          }
        }
        result += "\n";
      }
    }
  }
  return result;
}
} // namespace trading

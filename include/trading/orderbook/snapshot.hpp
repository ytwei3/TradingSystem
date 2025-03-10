#ifndef TRADING_ORDERBOOK_SNAPSHOT_HPP_
#define TRADING_ORDERBOOK_SNAPSHOT_HPP_

#include "trading/type.hpp"

#include <string>
#include <vector>

namespace trading {

struct PriceQuantity {
  Price price;
  Quantity quantity;
};

struct Snapshot {
  std::string symbol;
  Timestamp epoch;

  std::vector<PriceQuantity> ask_price_levels;
  std::vector<PriceQuantity> bid_price_levels;

  Price last_trade_price;
  Quantity last_trade_quantity;
};
} // namespace trading

#endif // TRADING_ORDERBOOK_SNAPSHOT_HPP_

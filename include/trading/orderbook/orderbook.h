#ifndef TRADING_ORDERBOOK_ORDERBOOK_H
#define TRADING_ORDERBOOK_ORDERBOOK_H

#include "trading/orderbook/order.h"
#include "trading/type.h"

#include <map>
#include <vector>

namespace trading {

struct PriceLevelInfo {
  Quantity total_quantity;
  std::vector<Order> orders;
};

class OrderBook {
public:
  using BidOrderBookSide = std::map<Price, PriceLevelInfo, std::greater<Price>>;
  using AskOrderBookSide = std::map<Price, PriceLevelInfo, std::less<Price>>;
  BidOrderBookSide bids_;
  AskOrderBookSide asks_;

  Price GetBidPrice(int level) const;
  Price GetAskPrice(int level) const;
  Quantity GetBidQuantity(int level) const;
  Quantity GetAskQuantity(int level) const;

  const BidOrderBookSide GetBidIOrderBook() const { return bids_; }
  const AskOrderBookSide GetAskIOrderBook() const { return asks_; }

  auto AddOrder(const Order &order) -> void;
  auto DeleteOrder(const Order &order) -> void;

  // template <typename T> auto &GetOrderBook();
};
} // namespace trading

#endif // TRADING_ORDERBOOK_ORDERBOOK_H

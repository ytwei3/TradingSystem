#ifndef TRADING_ORDERBOOK_ORDERBOOK_HPP_
#define TRADING_ORDERBOOK_ORDERBOOK_HPP_

#include "trading/type.hpp"

#include <map>
#include <vector>

namespace trading {

struct Order {
  OrderId order_id;
  Price price;
  Quantity quantity;
};

struct PriceLevelInfo {
  Quantity total_quantity;
  std::vector<Order> orders;
};

class OrderBook {
public:
  using BidOrderBookSide = std::map<Price, PriceLevelInfo, std::greater<Price>>;
  using AskOrderBookSide = std::map<Price, PriceLevelInfo>;
  BidOrderBookSide bids_;
  AskOrderBookSide asks_;

  Price GetBidPrice(int level) const;
  Price GetAskPrice(int level) const;
  Quantity GetBidQuantity(int level) const;
  Quantity GetAskQuantity(int level) const;

  const BidOrderBookSide GetBidIOrderBook() const { return bids_; }
  const AskOrderBookSide GetAskIOrderBook() const { return asks_; }
  // template <typename T> auto &GetOrderBook();
};
} // namespace trading

#endif // TRADING_ORDERBOOK_ORDERBOOK_HPP_

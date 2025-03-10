#include "trading/orderbook/orderbook.hpp"

namespace trading {

double OrderBook::GetBidPrice(int level) const {
  auto &book = bids_;

  if (book.size() <= level) {
    return 0;
  }
  auto it = book.begin();
  std::advance(it, level);
  return it->first;
}

double OrderBook::GetAskPrice(int level) const {
  auto &book = asks_;

  if (book.size() <= level) {
    return 0;
  }
  auto it = book.begin();
  std::advance(it, level);
  return it->first;
}

int OrderBook::GetBidQuantity(int level) const {
  auto &book = bids_;
  if (book.size() <= level) {
    return 0;
  }
  auto it = book.begin();
  std::advance(it, level);
  return it->second.total_quantity;
}

int OrderBook::GetAskQuantity(int level) const {
  auto &book = asks_;
  if (book.size() <= level) {
    return 0;
  }
  auto it = book.begin();
  std::advance(it, level);
  return it->second.total_quantity;
}
} // namespace trading
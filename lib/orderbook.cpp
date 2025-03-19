#include "trading/orderbook/orderbook.h"

namespace trading {

template <class T>
typename T::iterator AddOrder(T &side, Price price, Quantity quantity) {
  auto [it, inserted] = side.try_emplace(price, PriceLevelInfo{0, {}});
  if (inserted == false) {
    it->second.total_quantity += quantity;
  }
  return it;
}

template <class T>
typename T::iterator DeleteOrder(T &side, Price price, Quantity quantity) {
  auto it = side.find(price);
  if (it != side.end()) {
    it->second.total_quantity -= quantity;
    if (it->second.total_quantity == 0) {
      side.erase(it);
    }
  }
}

auto OrderBook::GetBidPrice(int level) const -> Price {
  auto &book = bids_;

  if (book.size() <= static_cast<decltype(book.size())>(level)) {
    return 0;
  }
  auto it = book.begin();
  std::advance(it, level);
  return it->first;
}

auto OrderBook::GetAskPrice(int level) const -> Price {
  auto &book = asks_;

  if (book.size() <= static_cast<decltype(book.size())>(level)) {
    return 0;
  }
  auto it = book.begin();
  std::advance(it, level);
  return it->first;
}

auto OrderBook::GetBidQuantity(int level) const -> Quantity {
  auto &book = bids_;
  if (book.size() <= static_cast<decltype(book.size())>(level)) {
    return 0;
  }
  auto it = book.begin();
  std::advance(it, level);
  return it->second.total_quantity;
}

auto OrderBook::GetAskQuantity(int level) const -> Quantity {
  auto &book = asks_;
  if (book.size() <= static_cast<decltype(book.size())>(level)) {
    return 0;
  }
  auto it = book.begin();
  std::advance(it, level);
  return it->second.total_quantity;
}
} // namespace trading
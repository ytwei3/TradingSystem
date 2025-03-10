#include <cassert>
#include <iostream>

#include "trading/processor/processor.hpp"

int main() {
  trading::OrderBookProcessor processor;
  processor.ReadMarketData("../data/cancel_test_data.log");

  std::string symbol{"TSLA"};

  // Test cancel orders in different price level
  processor.ProcessMarketData(symbol, 4);
  {
    auto &orderbook = processor.GetOrderBook(symbol);
    auto &bids = orderbook.GetBidIOrderBook();
    auto &asks = orderbook.GetAskIOrderBook();

    assert(bids.size() == 2);
    assert(asks.size() == 1);

    assert(bids.begin()->second.total_quantity == 4);
    assert(bids.begin()->second.orders.size() == 1);

    assert(asks.begin()->second.total_quantity == 15);
    assert(asks.begin()->second.orders.size() == 2);
  }

  processor.ProcessMarketData(symbol, 5);
  {
    auto &orderbook = processor.GetOrderBook(symbol);
    auto &bids = orderbook.GetBidIOrderBook();
    auto &asks = orderbook.GetAskIOrderBook();

    assert(bids.size() == 1);
    assert(asks.size() == 1);
  }

  // test cancel orders in the same price level
  processor.ProcessMarketData(symbol, 6);
  {
    auto &orderbook = processor.GetOrderBook(symbol);
    auto &bids = orderbook.GetBidIOrderBook();
    auto &asks = orderbook.GetAskIOrderBook();

    assert(asks.begin()->second.total_quantity == 10);
    assert(asks.begin()->second.orders.size() == 1);
  }

  std::cout << "Test passed!";

  return 0;
}
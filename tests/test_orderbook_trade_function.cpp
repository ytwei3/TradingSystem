#include <cassert>
#include <iostream>

#include "trading/processor/processor.hpp"

int main() {
  trading::OrderBookProcessor processor;
  processor.ReadMarketData("../data/trade_test_data.log");

  std::string symbol{"APPL"};

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

    assert(asks.size() == 1);
    assert(asks.begin()->second.total_quantity == 11);
    assert(asks.begin()->second.orders.size() == 2);
    assert(asks.begin()->second.orders[0].initilized_quantity == 1);

    // check last trade in snapshot
    auto &snapshot = processor.snapshot_cache_.at(symbol);
    assert(snapshot.back().last_trade_price == 9.7);
    assert(snapshot.back().last_trade_quantity == 4);
  }

  // test cancel orders in the same price level
  processor.ProcessMarketData(symbol, 6);
  {
    auto &orderbook = processor.GetOrderBook(symbol);
    auto &bids = orderbook.GetBidIOrderBook();
    auto &asks = orderbook.GetAskIOrderBook();

    assert(bids.size() == 1);
    assert(bids.begin()->first == 9.5);
    assert(bids.begin()->second.total_quantity == 6);

    auto &snapshot = processor.snapshot_cache_.at(symbol);
    assert(snapshot.back().last_trade_price == 9.6);
    assert(snapshot.back().last_trade_quantity == 4);
  }

  std::cout << "Test passed!";

  return 0;
}
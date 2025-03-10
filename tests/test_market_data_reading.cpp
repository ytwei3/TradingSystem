#include <cassert>
#include <iostream>

#include "trading/processor/processor.hpp"
#include "trading/type.hpp"

int main() {
  trading::OrderBookProcessor processor;
  processor.ReadMarketData("../data/general_test_data.log");

  std::string symbol{"APPL"};
  auto loaded_order_data = processor.data_records_[symbol];

  // sell new record
  // 1    1   APPL SELL NEW 10.11 1
  auto first_data = loaded_order_data.front();
  loaded_order_data.pop();

  assert(first_data.timestamp == 1);
  assert(first_data.order_id == 1);
  assert(first_data.symbol == symbol);
  assert(first_data.side == trading::TransactionSide::SELL);
  assert(first_data.category == trading::TransactionCategory::NEW);
  assert(first_data.price == 10.11);
  assert(first_data.quantity == 1);

  // buy new record
  // 1    2   APPL BUY  NEW 10.10 1
  auto second_data = loaded_order_data.front();
  loaded_order_data.pop();

  assert(second_data.timestamp == 1);
  assert(second_data.order_id == 2);
  assert(second_data.symbol == symbol);
  assert(second_data.side == trading::TransactionSide::BUY);
  assert(second_data.category == trading::TransactionCategory::NEW);
  assert(second_data.price == 10.10);
  assert(second_data.quantity == 1);

  // cancel record
  // 3    1   APPL SELL CANCEL 10.11 1
  loaded_order_data.pop();
  loaded_order_data.pop();
  auto cancel_data = loaded_order_data.front();
  loaded_order_data.pop();

  assert(cancel_data.timestamp == 3);
  assert(cancel_data.order_id == 1);
  assert(cancel_data.symbol == symbol);
  assert(cancel_data.side == trading::TransactionSide::SELL);
  assert(cancel_data.category == trading::TransactionCategory::CANCEL);
  assert(cancel_data.price == 10.11);
  assert(cancel_data.quantity == 1);

  // trade with large number record
  // 1609732196570507378    7374421476726859016   APPL BUY  TRADE 10.10 1

  auto trade_data = loaded_order_data.front();
  loaded_order_data.pop();

  assert(trade_data.timestamp == 1609732196570507378);
  assert(trade_data.order_id == 7374421476726859016);
  assert(trade_data.symbol == symbol);
  assert(trade_data.side == trading::TransactionSide::BUY);
  assert(trade_data.category == trading::TransactionCategory::TRADE);
  assert(trade_data.price == 10.11);
  assert(trade_data.quantity == 1);

  return 0;
}
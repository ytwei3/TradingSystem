#include "trading/processor/processor.h"
#include "trading/orderbook/orderbook.h"
#include "trading/orderbook/snapshot.h"
#include "trading/type.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace trading {

TransactionSide parse_side(std::string_view side) {
  if (side == "BUY") {
    return TransactionSide::BUY;
  } else {
    return TransactionSide::SELL;
  }
}

TransactionCategory parse_category(std::string_view category) {
  if (category == "NEW") {
    return TransactionCategory::NEW;
  } else if (category == "CANCEL") {
    return TransactionCategory::CANCEL;
  } else {
    return TransactionCategory::TRADE;
  }
}

void OrderBookProcessor::ReadMarketData(const std::string &filename) {
  std::ifstream input_file(filename);

  // check if file is found
  if (!input_file.good()) {
    std::cerr << "File not found: " << filename << std::endl;
    return;
  }

  std::string line;
  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    MarketDataRecord record;
    std::string side;
    std::string category;

    if (iss >> record.timestamp >> record.order_id >> record.symbol >> side >>
        category >> record.price >> record.quantity) {
      record.side = parse_side(side);
      record.category = parse_category(category);
      data_records_[record.symbol].push(std::move(record));
    } else {
      std::cerr << "Error reading line: " << line << std::endl;
    }
  }
}

// build to certain time point
void OrderBookProcessor::ProcessMarketData(const std::string &symbol,
                                           Timestamp timestamp) {
  std::cout << "Building orderbook to timestamp: " << timestamp << std::endl;
  processed_timestamp_ = timestamp;

  double last_total_trade_price = 0; // should be vwap
  int last_trade_quantity = 0;

  auto &symbol_data = data_records_[symbol];
  while (!symbol_data.empty() && symbol_data.front().timestamp <= timestamp) {
    MarketDataRecord data = symbol_data.front();
    symbol_data.pop();

    if (data.category == TransactionCategory::NEW) {
      if (data.side == TransactionSide::BUY) {
        ProcessBuyNewRecord(std::move(data));
      } else {
        ProcessSellNewRecord(std::move(data));
      }
    } else if (data.category == TransactionCategory::CANCEL) {
      if (data.side == TransactionSide::BUY) {
        ProcessBuyCancelRecord(std::move(data));
      } else {
        ProcessSellCancelRecord(std::move(data));
      }
    } else {
      if (data.side == TransactionSide::BUY) {
        ProcessBuyTradeRecord(std::move(data));
      } else {
        ProcessSellTradeRecord(std::move(data));
      }
      last_total_trade_price += data.price * data.quantity;
      last_trade_quantity += data.quantity;
    }

    // save this snapshot of the orderbook
    if (symbol_data.empty() || symbol_data.front().timestamp > data.timestamp) {
      auto last_trade_price = last_total_trade_price / last_trade_quantity;
      SaveSnapshot(symbol, data.timestamp, last_trade_price,
                   last_trade_quantity);

      last_total_trade_price = 0;
      last_trade_quantity = 0;
    }
  }
}

void OrderBookProcessor::ProcessBuyNewRecord(const MarketDataRecord &&data) {
  auto &price_level = orderbook_cache_[data.symbol].bids_[data.price];

  price_level.total_quantity += data.quantity;
  price_level.orders.emplace_back(
      Order{data.order_id, data.price, data.quantity});
}

void OrderBookProcessor::ProcessSellNewRecord(const MarketDataRecord &&data) {
  auto &price_level = orderbook_cache_[data.symbol].asks_[data.price];

  price_level.total_quantity += data.quantity;
  price_level.orders.emplace_back(
      Order{data.order_id, data.price, data.quantity});
}

void OrderBookProcessor::ProcessBuyCancelRecord(const MarketDataRecord &&data) {
  auto &book = orderbook_cache_[data.symbol].bids_;
  auto &orders = book.at(data.price).orders;

  for (auto it = orders.begin(); it != orders.end(); ++it) {
    if (it->order_id == data.order_id) {
      orders.erase(it);
      book[data.price].total_quantity -= data.quantity;

      if (orders.empty()) {
        book.erase(data.price);
      }
      break;
    }
  }
}

void OrderBookProcessor::ProcessSellCancelRecord(
    const MarketDataRecord &&data) {}

void OrderBookProcessor::ProcessBuyTradeRecord(const MarketDataRecord &&data) {
  auto &traded_book = orderbook_cache_[data.symbol].bids_;

  auto data_quantity = data.quantity;
  while (data_quantity > traded_book.begin()->second.total_quantity) {
    data_quantity -= traded_book.begin()->second.total_quantity;
    traded_book.erase(traded_book.begin());
  }

  if (data_quantity == traded_book.begin()->second.total_quantity) {
    traded_book.erase(traded_book.begin());
    return;
  }

  auto &orders = traded_book.begin()->second.orders;
  for (auto it = orders.begin(); it != orders.end(); ++it) {
    if (data_quantity >= it->initilized_quantity) {
      data_quantity -= it->initilized_quantity;
      traded_book.begin()->second.total_quantity -= it->initilized_quantity;
      it = orders.erase(it);
    } else {
      it->initilized_quantity -= data_quantity;
      traded_book.begin()->second.total_quantity -= data_quantity;
      break;
    }
  }
}

void OrderBookProcessor::ProcessSellTradeRecord(const MarketDataRecord &&data) {
  auto &traded_book = orderbook_cache_[data.symbol].asks_;

  auto data_quantity = data.quantity;
  while (data_quantity > traded_book.begin()->second.total_quantity) {
    data_quantity -= traded_book.begin()->second.total_quantity;
    traded_book.erase(traded_book.begin());
  }

  if (data_quantity == traded_book.begin()->second.total_quantity) {
    traded_book.erase(traded_book.begin());
    return;
  }

  auto &orders = traded_book.begin()->second.orders;
  for (auto it = orders.begin(); it != orders.end(); ++it) {
    if (data_quantity >= it->initilized_quantity) {
      data_quantity -= it->initilized_quantity;
      traded_book.begin()->second.total_quantity -= it->initilized_quantity;
      it = orders.erase(it);
    } else {
      it->initilized_quantity -= data_quantity;
      traded_book.begin()->second.total_quantity -= data_quantity;
      break;
    }
  }
}

void OrderBookProcessor::SaveSnapshot(const std::string &symbol,
                                      Timestamp timestamp,
                                      Price last_trade_price,
                                      Quantity last_trade_quantity) {
  Snapshot snapshot;
  snapshot.symbol = symbol;
  snapshot.epoch = timestamp;

  for (int i = 0; i < 5; ++i) {
    snapshot.ask_price_levels.emplace_back(
        PriceQuantity{orderbook_cache_[symbol].GetAskPrice(i),
                      orderbook_cache_[symbol].GetAskQuantity(i)});

    snapshot.bid_price_levels.emplace_back(
        PriceQuantity{orderbook_cache_[symbol].GetBidPrice(i),
                      orderbook_cache_[symbol].GetBidQuantity(i)});
  }

  snapshot.last_trade_price = last_trade_price;
  snapshot.last_trade_quantity = last_trade_quantity;

  snapshot_cache_[symbol].emplace_back(std::move(snapshot));
}

} // namespace trading
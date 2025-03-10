#ifndef TRADING_PROCESSOR_PROCESSOR_HPP_
#define TRADING_PROCESSOR_PROCESSOR_HPP_

#include "trading/orderbook/orderbook.hpp"
#include "trading/orderbook/snapshot.hpp"
#include "trading/processor/market_data_record.hpp"

#include <queue>
#include <string>
#include <unordered_map>

namespace trading {

class OrderBookProcessor {
  /**
   * @brief process record which category is NEW
   *
   * @param data market data record, one line of the input file
   */
  void ProcessBuyNewRecord(const MarketDataRecord &&data);
  void ProcessSellNewRecord(const MarketDataRecord &&data);

  /**
   * @brief process record which category is CANCEL
   *
   * @param data market data record, one line of the input file
   */
  void ProcessBuyCancelRecord(const MarketDataRecord &&data);
  void ProcessSellCancelRecord(const MarketDataRecord &&data);

  /**
   * @brief process record which category is TRADE
   *
   * @param data market data record, one line of the input file
   */
  void ProcessBuyTradeRecord(const MarketDataRecord &&data);
  void ProcessSellTradeRecord(const MarketDataRecord &&data);

  /**
   * @brief save snapshot to cache
   *
   * @param symbol symbol
   * @param timestamp timestamp
   * @param last_trade_price last trade price
   * @param last_trade_quantity last trade quantity
   */
  void SaveSnapshot(const std::string &symbol, Timestamp timestamp,
                    Price last_trade_price, Quantity last_trade_quantity);

public:
  OrderBookProcessor() = default;
  ~OrderBookProcessor() {
    data_records_.clear();
    snapshot_cache_.clear();
  }

  using OrderBookCache = std::unordered_map<std::string, OrderBook>;
  using SnapshotCache = std::unordered_map<std::string, std::vector<Snapshot>>;
  using MarketDataRecordCache =
      std::unordered_map<std::string, std::queue<MarketDataRecord>>;

  SnapshotCache snapshot_cache_;
  MarketDataRecordCache data_records_;

  OrderBookCache orderbook_cache_;
  Timestamp processed_timestamp_ = 0;

  /**
   * @brief read market data from file
   * @param filename file path
   */
  void ReadMarketData(const std::string &filename);

  /**
   * @brief process market data to a certain timestamp
   * @param symbol symbol
   * @param timestamp timestamp
   */
  void ProcessMarketData(const std::string &symbol, Timestamp timestamp);

  const OrderBook GetOrderBook(const std::string &symbol) const {
    return orderbook_cache_.at(symbol);
  }
  const Timestamp GetProcessedTimestamp() const { return processed_timestamp_; }
  // template <typename T> auto ProcessNewRecord(const MarketDataRecord &&data);
};
} // namespace trading

#endif // TRADING_PROCESSOR_PROCESSOR_HPP_

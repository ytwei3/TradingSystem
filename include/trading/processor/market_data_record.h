#ifndef TRADING_PROCESSOR_MARKET_DATA_RECORD_H
#define TRADING_PROCESSOR_MARKET_DATA_RECORD_H

#include "trading/type.h"

#include <string>

namespace trading {

struct MarketDataRecord {
public:
  Timestamp timestamp;
  OrderId order_id;
  std::string symbol;
  TransactionSide side;
  TransactionCategory category;
  Price price;
  Quantity quantity;
};

} // namespace trading

#endif // TRADING_PROCESSOR_MARKET_DATA_RECORD_H

#ifndef TRADING_TYPE_HPP_
#define TRADING_TYPE_HPP_

namespace trading {

using Price = double;
using Quantity = int;
using OrderId = long long;
using Timestamp = long long;

enum class TransactionSide { BUY, SELL };
enum class TransactionCategory { NEW, CANCEL, TRADE };

} // namespace trading

#endif // TRADING_TYPE_HPP_

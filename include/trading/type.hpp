#ifndef TRADING_TYPE_HPP_
#define TRADING_TYPE_HPP_

#include <cstdint>

namespace trading {

using Price = std::uint32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint64_t;
using Timestamp = std::uint64_t;

enum class TransactionSide { BUY, SELL };
enum class TransactionCategory { NEW, CANCEL, TRADE };

} // namespace trading

#endif // TRADING_TYPE_HPP_

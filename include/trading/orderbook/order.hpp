#ifndef TRADING_ORDERBOOK_ORDER_HPP_
#define TRADING_ORDERBOOK_ORDER_HPP_

#include "trading/type.hpp"

namespace trading {

class Order {
public:
  OrderId order_id;
  Price price;
  Quantity initilized_quantity;
  Quantity unfilled_quantity;

  Order(OrderId order_id, Price price, Quantity quantity)
      : order_id(order_id), price(price), initilized_quantity(quantity),
        unfilled_quantity(quantity) {}

  OrderId GetOrderId() const { return order_id; }
  Price GetPrice() const { return price; }
  Quantity GetInitilizedQuantity() const { return initilized_quantity; }
  Quantity GetUnfilledQuantity() const { return unfilled_quantity; }

  void ReduceQuantity(Quantity quantity) { unfilled_quantity -= quantity; }

  bool IsFilled() const { return unfilled_quantity == 0; }
};
} // namespace trading

#endif // TRADING_ORDERBOOK_ORDER_HPP_
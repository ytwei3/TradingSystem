#include <cassert>
#include <iostream>

#include "trading/processor/processor.hpp"
#include "trading/type.hpp"

int main() {
  trading::OrderBookProcessor processor;
  processor.ReadMarketData("../data/APPL.log");

  processor.ProcessMarketData("APPL", 1);

  return 0;
}
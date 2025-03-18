#include <cassert>
#include <iostream>

#include "trading/processor/processor.h"
#include "trading/type.h"

int main() {
  trading::OrderBookProcessor processor;
  processor.ReadMarketData("../data/APPL.log");

  processor.ProcessMarketData("APPL", 1);

  return 0;
}
## Execute instruction
---

#### Development Environment

- **MacOS 14.4.1**
- **CMake v3.28.0**  - minimum requirement: 3.20
- **Clang v17.0.6**  - compiler needs to support at least the C++17 standard

#### Buliding

To build this project, run follows in the project root directory:

```
mkdir build && cd build
cmake ..
make
```

#### Executing

Then, you'll find a generated `/bin` directory under root. Now you can access through command line under `/bin`:
```
./main
```
****
#### Testing
After builiding, in `/bulid` run:
```
ctest
```

## Usage

#### Query method

To perform query, you should write query directly in cpp source file. the query syntax is:
```C++
  auto result = engine.queryOrderBookSnapshots(
      processor,
      {"SCS", "SCH"},   // symbols
      0,                // start time
      6,                // end time
      {3, 2, 1},        // bids index level
      {1, 2, 3},        // asks index level
      true,             // whether to output last trade price
      true              // whether to output last trade quantity
    );
```

#### Example

This is a quick example in `src/main.cpp`:

```C++
#include <iostream>

#include "trading/execution/engine.hpp"
#include "trading/processor/processor.hpp"

int main() {
  trading::OrderBookProcessor processor;
  processor.ReadMarketData("./data/trade_test_data.log");

  std::vector<std::string> symbol{"APPL"};

  processor.ProcessMarketData(symbol[0], 6);

  auto engine = trading::ExecutionEngine();

  auto result = engine.queryOrderBookSnapshots(
      processor, symbol, 0, 6, {3, 2, 1}, {1, 2, 3}, true, true);

  std::cout << result << std::endl;

  return 0;
}

```

And the result of example should be:

```
symbol, epoch, bid3q@bid3p bid2q@bid2p bid1q@bid1p X ask1q@ask1p ask2q@ask2p ask3q@ask3p, last trade price, last trade quantity
APPL, 1, 0@0.000 0@0.000 4@9.600 X 0@0.000 0@0.000 0@0.000, nan, NA
APPL, 2, 0@0.000 6@9.500 4@9.600 X 0@0.000 0@0.000 0@0.000, nan, NA
APPL, 3, 0@0.000 6@9.500 4@9.600 X 5@9.700 0@0.000 0@0.000, nan, NA
APPL, 4, 0@0.000 6@9.500 4@9.600 X 15@9.700 0@0.000 0@0.000, nan, NA
APPL, 5, 0@0.000 6@9.500 4@9.600 X 11@9.700 0@0.000 0@0.000, 9.700000, 4
APPL, 6, 0@0.000 0@0.000 6@9.500 X 11@9.700 0@0.000 0@0.000, 9.600000, 4
```

## Design Principle
---

The desgin is mainly modularized in three parts, just acts like the question description:

- OrderBook
- Processor
- Execution Engine

#### OrderBook

1. **price-level data storage**. I have utilized the `std::map` data structure to store price levels based on their prices. This choice allows for sorted ordering of the price levels, ensuring efficient **O(logn)** time complexity for price queries. An alternative approach could have been using `std::vector`, which offers O(1) complexity for data append and deletion. However, finding a specific price level using a vector would have taken **O(n)** time. Thus, the vector data structure would have been more suitable for situations involving a small number of price levels or frequent changes in price levels.
2. **order storage in same price level.** I choose `std::vector`. With FIFO feature, `std::queue` could be a choice, but when perform CANCEL order operation, `std::queue` has to pop a lot of times, which is costly.

#### Processor

This processor achieved three operations for Orderbook: `NEW`, `CANCEL`, `TRADE`

- `NEW` operation: 
  - Find order in its price level.
  - Push it in the back of `Orders` 
- `CANCEL` operation:
  - Find order to be canceled by price level first, then find the order with same *order_id* in the `Orders`, erase it
- `TRADE` operation:
  - Trade with the best price level first, then with `Orders` in FIFO order.
  - if the quantity at that price level is 0, erase this level in `OrderBook`.

Apart from that, the processor will store **current state** of orderbook and **snapshot** for optimization on multi query. This means snapshot will only be processed once if we query **0-1000** then **0-500**, snapshot will only be processed 1000 times.


#### Engine

The engine is relatively simple. As we only define what type of output we want, I just use a for-loop to find the field that satisfied demand, concat it, and finally print it. The result format should pretty similar as the description.

For implementation, when perform snapshot query, the `ExecutionEngine` will first look at whether the `Processor` has stored snapshot cache:
- if yes, `ExectuionEngine` will find data from `SnapShotCache` in `Processor`. 

- if no, `ExectuionEngine` will call `Processor` will process data, up to the lastest time as query needed. 

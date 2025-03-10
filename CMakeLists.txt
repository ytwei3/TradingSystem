cmake_minimum_required(VERSION 3.20)

project(
  "TradingSystem"
  VERSION 0.1.0
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()
message(STATUS "Started CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


# ------------------- Add libraries -------------------
add_library(Trading
  src/orderbook.cpp
  src/processor.cpp
  src/engine.cpp
)
target_include_directories(Trading PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(main src/main.cpp)

target_link_libraries(main Trading)

# ------------------- Add tests -------------------
# testing code
enable_testing()

add_executable(test_orderbook tests/test_orderbook.cpp)
target_link_libraries(test_orderbook Trading)
add_test(NAME test_orderbook COMMAND test_orderbook --verbose)

add_executable(test_engine tests/test_engine.cpp)
target_link_libraries(test_engine Trading)
add_test(NAME test_engine COMMAND test_engine --verbose)

add_executable(test_market_data_reading tests/test_market_data_reading.cpp)
target_link_libraries(test_market_data_reading Trading)
add_test(NAME test_market_data_reading COMMAND test_market_data_reading --verbose)

add_executable(test_orderbook_trade_function tests/test_orderbook_trade_function.cpp)
target_link_libraries(test_orderbook_trade_function Trading)
add_test(NAME test_orderbook_trade_function COMMAND test_orderbook_trade_function --verbose)
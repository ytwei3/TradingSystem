cmake_minimum_required(VERSION 3.20)

project(
  "TradingSystem"
  VERSION 0.1.0
)

# cpp warnings
if(MSVC)
    add_compile_options("/Zc:__cplusplus")
    add_compile_options("/Zc:preprocessor")
    add_compile_options("/MP")
else()
    add_compile_options(-Wall -Wextra -Werror -Wno-unused-parameter -Wno-missing-field-initializers)

    add_compile_options($<$<CONFIG:Debug>:-fsanitize=undefined>)
    add_link_options($<$<CONFIG:Debug>:-fsanitize=undefined>)

    # macos with gcc11 has bugs in asan:
    # `member call on address 0x60b0000001a0 which does not point to an object of type '_Sp_counted_base'`
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(STATUS "Enable asan")

        add_compile_options($<$<CONFIG:Debug>:-fsanitize=address>)
        add_link_options($<$<CONFIG:Debug>:-fsanitize=address>)
    endif()

	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_compile_options(-Wno-restrict)
		add_compile_options(-Wno-free-nonheap-object)
		add_compile_options(-Wno-dangling-reference)
	endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

message(STATUS "Started CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 23)

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


# ------------------- Add libraries -------------------
add_library(Trading
  src/orderbook.cpp
  src/processor.cpp
  src/engine.cpp
  src/logger.cpp
)
target_include_directories(Trading PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(main src/main.cpp)

target_link_libraries(main PRIVATE Trading)

# ------------------- Add tests -------------------
if(ENABLE_TEST)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

# testing code

add_executable(test_draft tests/draft.cpp)
target_link_libraries(test_draft Trading)
add_test(NAME test_draft COMMAND test_processor --verbose)

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
file(GLOB lib_srcs ${CMAKE_CURRENT_SOURCE_DIR}/**.cpp)
file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/*/**.cpp)

message(STATUS "lib_srcs: ${lib_srcs}")
message(STATUS "srcs: ${srcs}")

add_library(trading_lib ${lib_srcs} ${srcs})
set_target_properties(trading_lib PROPERTIES OUTPUT_NAME "trading")

target_include_directories(trading_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(trading_lib PUBLIC "${CMAKE_SOURCE_DIR}/include")
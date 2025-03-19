add_executable(main main.cpp)

target_compile_definitions(main PRIVATE TRADING_SYSTEM_VERSION="${PROJECT_VERSION}")
target_link_libraries(main PRIVATE trading_lib)

# install(TARGETS main)
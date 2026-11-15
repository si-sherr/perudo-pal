project(test-parser)

add_executable(test-parser test_parser.cpp
               ${CMAKE_SOURCE_DIR}/src/cli/parser/parser.c)

target_link_libraries(test-parser GTest::gtest_main ppal-core)

target_include_directories(
  test-parser
  PRIVATE ${CMAKE_SOURCE_DIR}/src/cli/parser)

gtest_discover_tests(test-parser)

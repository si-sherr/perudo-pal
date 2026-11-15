project(test-handler)

add_executable(test-handler test_handler.cpp
               ${CMAKE_SOURCE_DIR}/src/cli/handler/handler.c)

target_link_libraries(test-handler GTest::gtest_main fff ppal-core)

target_include_directories(
  test-handler
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src/cli/handler ${CMAKE_SOURCE_DIR}/src/version
    ${CMAKE_SOURCE_DIR}/tests/fakes)

gtest_discover_tests(test-handler)

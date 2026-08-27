project(perudo-pal-tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  ppal-tests
  hello_test.cpp
)

target_link_libraries(
  ppal-tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ppal-tests)
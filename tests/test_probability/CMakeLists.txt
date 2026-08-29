project(test-probability)

add_executable(test-probability
    test_probability.cpp
)

target_link_libraries(test-probability
    GTest::gtest_main
    ppal-core
)

gtest_discover_tests(test-probability)


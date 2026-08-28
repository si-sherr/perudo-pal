project(test-probability)

add_executable(test-probability
    ${CMAKE_SOURCE_DIR}/src/probability/probability.c
    test_probability.cpp

)

target_link_libraries(test-probability
    GTest::gtest_main
)

target_include_directories(test-probability
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/probability
)

gtest_discover_tests(test-probability)


project(test-game-engine)

add_executable(test-game-engine test_game_engine.cpp)

target_link_libraries(test-game-engine GTest::gtest_main fff ppal-core)

target_include_directories(
  test-game-engine PRIVATE ${CMAKE_SOURCE_DIR}/tests/external/
                           ${CMAKE_SOURCE_DIR}/tests/fakes/)

gtest_discover_tests(test-game-engine)

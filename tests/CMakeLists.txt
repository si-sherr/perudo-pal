project(perudo-pal-tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

enable_testing()

add_subdirectory(test_probability)
add_subdirectory(test_game_engine)
add_subdirectory(test_parser)

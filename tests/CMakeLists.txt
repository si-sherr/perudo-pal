project(perudo-pal-tests)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip)

# Fetch just the needed header from source. The repo itself pulls in it's own
# gtest files and I think this causes issues when building tests
FetchContent_Declare(
  fff
  URL https://github.com/meekrosoft/fff/releases/download/v1.1/fff.h
  DOWNLOAD_NO_EXTRACT TRUE)

FetchContent_MakeAvailable(googletest fff)

add_library(fff INTERFACE)
target_include_directories(fff INTERFACE ${fff_SOURCE_DIR})

include(GoogleTest)
enable_testing()

add_subdirectory(test_probability)
add_subdirectory(test_game_engine)
add_subdirectory(test_parser)
add_subdirectory(test_handler)

project(perudo-pal-cli)

set(SOURCES main.c parser/parser.c handler/handler.c)

add_compile_options(-Wall -Wformat=2)

add_executable(ppal ${SOURCES})

add_dependencies(ppal version)

target_link_libraries(ppal ppal-core)

target_include_directories(
  ppal
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/parser ${CMAKE_CURRENT_LIST_DIR}/handler
          ${CMAKE_SOURCE_DIR}/src/version)

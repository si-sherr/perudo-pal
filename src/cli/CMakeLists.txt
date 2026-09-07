project(perudo-pal-src)

set(SOURCES main.c)

add_compile_options(-Wall -Wformat=2)

add_executable(ppal ${SOURCES})

target_link_libraries(ppal ppal-core)

target_include_directories(ppal PRIVATE)
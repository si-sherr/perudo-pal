project(perudo-pal-src)

set(SOURCES
    main/main.c
    probability/probability.c
)

add_compile_options(-Wall -Wformat=2)

add_executable(ppal ${SOURCES})

# libm requires explicit linkage
target_link_libraries(ppal m)

target_include_directories(ppal
    PRIVATE
        main/
        probability/
)
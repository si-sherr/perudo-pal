set(SOURCES
    probability/probability.c
)

add_library(ppal-core 
    STATIC
        ${SOURCES}
)

# libm requires explicit linkage
target_link_libraries(ppal-core
    m
)

target_include_directories(ppal-core
    PUBLIC
        probability/
)
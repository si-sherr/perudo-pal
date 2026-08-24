project(perudo-pal-src)

set(SOURCES
    main/main.c
)

add_executable(ppal ${SOURCES})

target_include_directories(ppal
    PRIVATE
        main/
)
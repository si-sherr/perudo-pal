project(perudo-pal-src)

set(SOURCES
    main/main.c
)

add_compile_options(-Wall -Wformat=2)
add_executable(ppal ${SOURCES})
set_property(TARGET ppal PROPERTY C_STANDARD 23)

target_include_directories(ppal
    PRIVATE
        main/
)
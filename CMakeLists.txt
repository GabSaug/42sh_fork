cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c99 -pedantic")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O2")

set(SRC
    src/main.c
    src/my_malloc.c
    src/my_string.c
    src/lexer.c
    src/hash_table.c
    src/vector.c
    src/option_parser.c)

include_directories(src/)

add_executable(42sh ${SRC})

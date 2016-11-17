cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c99 -pedantic -g")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(BIN_NAME "42sh")

include_directories(src/util
                    src/lexer
                    src/execute
                    src/main
                    src/parser
                    src/rules
                    src/option_parser)

add_subdirectory(src/execute)
add_subdirectory(src/lexer)
add_subdirectory(src/main)
add_subdirectory(src/option_parser)
add_subdirectory(src/parser)
add_subdirectory(src/rules)
add_subdirectory(src/util)

add_executable(${BIN_NAME} ${SRC})
target_link_libraries(${BIN_NAME} readline)

add_custom_target(check
                  COMMAND ./test.rb -b ${CMAKE_BINARY_DIR}/${BIN_NAME}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)

add_custom_target(echeck
                  COMMAND ./test.rb -e -b ${CMAKE_BINARY_DIR}/${BIN_NAME}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)

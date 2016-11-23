cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c99 -pedantic -g")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(BIN_NAME "42sh")

include_directories(src/main
                    src/signals
                    src/option_parser
                    src/lexer
                    src/typer
                    src/rules
                    src/parser
                    src/expansion
                    src/execute
                    src/util)

add_subdirectory(src/main)
add_subdirectory(src/signals)
add_subdirectory(src/option_parser)
add_subdirectory(src/lexer)
add_subdirectory(src/typer)
add_subdirectory(src/rules)
add_subdirectory(src/parser)
add_subdirectory(src/expansion)
add_subdirectory(src/execute)
add_subdirectory(src/util)

add_executable(${BIN_NAME} ${SRC})
target_link_libraries(${BIN_NAME} readline)


add_custom_target(doc
                  COMMAND doxygen doxy &> /dev/null && chromium html/index.html
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc)


add_custom_target(check
                  COMMAND ./test.rb -b ${CMAKE_BINARY_DIR}/${BIN_NAME}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
                  DEPENDS ${BIN_NAME})

add_custom_target(echeck
                  COMMAND ./test.rb -e -b ${CMAKE_BINARY_DIR}/${BIN_NAME}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
                  DEPENDS ${BIN_NAME})

add_custom_target(scheck
                  COMMAND ./test.rb -s -b ${CMAKE_BINARY_DIR}/${BIN_NAME}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
                  DEPENDS ${BIN_NAME})

add_custom_target(escheck
                  COMMAND ./test.rb -s -e -b ${CMAKE_BINARY_DIR}/${BIN_NAME}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
                  DEPENDS ${BIN_NAME})

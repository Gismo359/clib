cmake_minimum_required(VERSION 4.0)
cmake_policy(SET CMP0091 NEW)

project(Gismo359::jizz LANGUAGES C)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES
    "${SOURCE_DIR}/*.c"
    "${SOURCE_DIR}/*.h"
    "${INCLUDE_DIR}/*.c"
    "${INCLUDE_DIR}/*.h"
)

add_library(clib STATIC ${SOURCES})
add_library(Gismo359::clib ALIAS clib)

target_include_directories(clib
    PRIVATE ${SOURCE_DIR}
    PUBLIC ${INCLUDE_DIR}
)

set_target_properties(clib
    PROPERTIES
    C_STANDARD 23
    C_EXTENSIONS ON
    UNITY_BUILD ON
)

cmake_minimum_required(VERSION 3.10)

project(strs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.c
    src/file.c
    src/strs.c
)

find_package(Curses REQUIRED)

add_executable(strs ${SOURCES})

include_directories(include)
include_directories(${CURSES_INCLUDE_DIR})

target_link_libraries(strs ${CURSES_LIBRARIES})

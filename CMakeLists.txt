cmake_minimum_required(VERSION 3.20)
project(netcalc)

set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
add_subdirectory(ctable)

set(SOURCES
    src/main.c
)

add_executable(netcalc ${SOURCES})

target_link_libraries(netcalc ctable)

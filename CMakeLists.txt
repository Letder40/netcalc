cmake_minimum_required(VERSION 3.20)
project(netcalc)

set(CMAKE_BUILD_TYPE release)

include_directories(include)
add_subdirectory(ctable)

set(SOURCES
    src/main.c
    src/args.c
    src/print_network.c
    src/vlsm.c
    src/flsm.c
)

add_executable(netcalc ${SOURCES})

target_link_libraries(netcalc ctable)

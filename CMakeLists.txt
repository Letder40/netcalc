cmake_minimum_required(VERSION 3.31)
project(netcalc)
set (CMAKE_C_COMPILER gcc)

make_directory(build)
file(MAKE_DIRECTORY build)

set(SOURCES
    src/main.c
    src/flsm.c
    src/vlsm.c
    src/flsm.c
)

add_executable(netcalc ${SOURCES})

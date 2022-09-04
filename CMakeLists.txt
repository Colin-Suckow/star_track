cmake_minimum_required(VERSION 3.2)

project (star_track)

file(GLOB_RECURSE SOURCES src/*.cpp)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(star_track ${SOURCES})

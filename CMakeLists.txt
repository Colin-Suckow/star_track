cmake_minimum_required(VERSION 3.2)

project (star_track)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(star_track ${SOURCES})

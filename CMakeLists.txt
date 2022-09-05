cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 14)

project (star_track)

set(LIB_SOURCES src/star_catalog.cpp src/star_catalog.hpp)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


add_library(star_track_lib ${LIB_SOURCES})

add_executable(star_track src/main.cpp)
target_link_libraries(star_track star_track_lib)

add_subdirectory(lib/Catch2)
add_executable(tests tests/main.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain star_track_lib)
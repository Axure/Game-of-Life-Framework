cmake_minimum_required(VERSION 3.4)
project(GameOfLif)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")

find_package(curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})


add_subdirectory(build)
cmake_minimum_required(VERSION 3.4)
project(GameOfLif)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

set(SOURCE_FILES main.cpp Game.cpp Game.h Screen.cpp Screen.h Buffer.h)
add_executable(GameOfLif ${SOURCE_FILES})
target_link_libraries(GameOfLif ${CURSES_LIBRARIES})
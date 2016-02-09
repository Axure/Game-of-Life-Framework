set(SOURCE_FILES
        ../../examples/GameOfLife/main.cpp
        ../../src/main/Buffer.h)
add_executable(GameOfLif ${SOURCE_FILES})
target_link_libraries(GameOfLif
        ${CURSES_LIBRARIES}
        Game
        Screen)

add_library(Screen
        ../../src/main/Screen.cpp
        ../../src/main/Screen.h
        ../../src/main/Buffer.h)

add_library(Game
        ../../src/main/Game.cpp
        ../../src/main/Game.h
        ../../src/main/Buffer.h)


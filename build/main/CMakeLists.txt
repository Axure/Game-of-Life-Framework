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
        ../../src/main/Buffer.h ../../src/main/utils/Any.cpp ../../src/main/utils/Any.h)
target_link_libraries(Screen
        ${CURSES_LIBRARIES}
        Timer
        )

add_library(Game
        ../../src/main/Game.cpp
        ../../src/main/Game.h
        ../../src/main/Buffer.h)

add_library(Logger
        ../../src/main/utils/Logger.cpp
        ../../src/main/utils/Logger.h
        ../../src/main/utils/LoggerFactory.cpp
        ../../src/main/utils/LoggerFactory.h ../../src/main/JoinedHeap.hpp)

add_library(Timer
        ../../src/main/utils/Timer.cpp
        ../../src/main/utils/Timer.h)
target_link_libraries(Timer Logger)

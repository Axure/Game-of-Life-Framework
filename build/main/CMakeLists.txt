set(SOURCE_FILES
        ../../examples/GameOfLife/main.cpp
        ../../src/main/DataStructure/Buffer.tcc)
add_executable(GameOfLif ${SOURCE_FILES})
target_include_directories(GameOfLif PUBLIC ../../src/main)
target_link_libraries(GameOfLif
        ${CURSES_LIBRARIES}
        Game
        Curses)

add_library(Curses
        ../../src/main/Engine/Curses.cpp
        ../../src/main/Engine/Curses.h
        ../../src/main/DataStructure/Buffer.tcc ../../src/main/DataStructure/Any.cpp ../../src/main/DataStructure/Any.h)
target_link_libraries(Curses
        ${CURSES_LIBRARIES}
        Timer
        )

add_library(Game
        ../../src/main/Game/Game.cpp
        ../../src/main/Game/Game.h
        ../../src/main/DataStructure/Buffer.tcc ../../src/main/DataStructure/TwoDimensionalGeometricBuffer.cpp ../../src/main/DataStructure/TwoDimensionalGeometricBuffer.hpp ../../src/main/Game/Dialog.cpp ../../src/main/Game/Dialog.h ../../src/main/Game/Control.cpp ../../src/main/Game/Control.h ../../src/main/Game/Canvas.cpp ../../src/main/Game/Canvas.h)

add_library(Logger
        ../../src/main/utils/Logger.cpp
        ../../src/main/utils/Logger.h
        ../../src/main/utils/LoggerFactory.cpp
        ../../src/main/utils/LoggerFactory.h ../../src/main/DataStructure/JoinedHeap.hpp)

add_library(Timer
        ../../src/main/utils/Timer.cpp
        ../../src/main/utils/Timer.h)
target_link_libraries(Timer Logger)

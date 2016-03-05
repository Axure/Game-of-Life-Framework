add_executable(BufferTest
        ../../src/test/BufferTest.cpp
        )
target_include_directories(BufferTest PUBLIC
        ../../src/main/)
target_link_libraries(BufferTest
        Logger)

add_executable(LoggerTest
        ../../src/test/LoggerTest.cpp
        )
target_include_directories(LoggerTest PUBLIC
        ../../src/main/)
target_link_libraries(LoggerTest
        Logger)

add_executable(ScreenTest
        ../../src/test/ScreenTest.cpp)
target_include_directories(ScreenTest PUBLIC
        ../../src/main/)
target_link_libraries(ScreenTest
        Curses
        Logger
        Timer)

add_executable(TimerTest
        ../../src/test/TimerTest.cpp)
target_include_directories(TimerTest PUBLIC
        ../../src/main/)
target_link_libraries(TimerTest
        Timer)

add_executable(GameTest
        ../../src/test/GameTest.cpp)
target_include_directories(GameTest PUBLIC
        ../../src/main/)
target_link_libraries(GameTest
        Game
        Curses
        Timer
        Logger
        )
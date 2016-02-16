add_executable(BufferTest
        ../../src/test/BufferTest.cpp)
target_include_directories(BufferTest PUBLIC
        ../../src/main/)
target_link_libraries(BufferTest
        Logger)

add_executable(ScreenTest
        ../../src/test/ScreenTest.cpp)
target_include_directories(ScreenTest PUBLIC
        ../../src/main/)
target_link_libraries(ScreenTest
        Screen
        Logger)

add_executable(TimerTest
        ../../src/test/TimerTest.cpp)
target_include_directories(TimerTest PUBLIC
        ../../src/main/)
target_link_libraries(TimerTest
        Timer)
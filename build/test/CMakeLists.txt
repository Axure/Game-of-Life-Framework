add_executable(BufferTest
        ../../src/test/BufferTest.cpp)
target_include_directories(BufferTest PUBLIC
        ../../src/main/)
target_link_libraries(BufferTest
        )
add_executable(TestMemory)

target_sources(TestMemory PRIVATE test_main.cpp)

target_link_libraries(TestMemory
    engine_lib
)


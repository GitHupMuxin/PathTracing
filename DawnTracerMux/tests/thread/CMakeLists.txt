add_executable(ThreadTest)

target_sources(ThreadTest PRIVATE test_main.cpp)

target_link_libraries(ThreadTest engine_lib)


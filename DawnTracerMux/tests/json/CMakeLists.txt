add_executable(TestJson)

target_sources(TestJson PRIVATE test_main.cpp)

target_link_libraries(TestJson
    engine_lib
)





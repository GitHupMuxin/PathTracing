add_executable(TestXML)

target_sources(TestXML PRIVATE test_main.cpp)

target_link_libraries(TestXML 
    engine_lib
    pugixml_lib
)

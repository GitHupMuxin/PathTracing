cmake_minimum_required(VERSION 3.15)
project(pugixml_lib LANGUAGES CXX)

add_library(pugixml_lib STATIC 
    source/pugixml.cpp
)

target_include_directories(pugixml_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(LIB_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/third_party/lib")

set_target_properties(pugixml_lib PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR}
)

cmake_minimum_required(VERSION 3.15)
project(PathTracing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native")

    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

    add_compile_definitions(NDEBUG)

endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(MY_COMPILE_NAME "GCC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(MY_COMPILE_NAME "Clang")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(MY_COMPILE_NAME "MSVC")
else()
    set(MY_COMPILE_NAME "other")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${MY_COMPILE_NAME}/${CMAKE_BUILD_TYPE}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${MY_COMPILE_NAME}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${MY_COMPILE_NAME}/${CMAKE_BUILD_TYPE}/lib)

option(BUILD_TESTS "Build test cases" ON)

add_subdirectory(third_party/pugixml)
add_subdirectory(engine)
add_subdirectory(app)


if(BUILD_TESTS)
    add_subdirectory(tests)
endif()



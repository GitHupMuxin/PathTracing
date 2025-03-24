cmake_minimum_required(VERSION 3.10)

project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

set(SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/BoundingBox.cpp
    ${CMAKE_SOURCE_DIR}/src/BoundingTree.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/Object.cpp
    ${CMAKE_SOURCE_DIR}/src/Ray.cpp
    ${CMAKE_SOURCE_DIR}/src/render.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/Tool.cpp
    

)


add_executable(RayTracing ${SRC_FILES})

target_include_directories(RayTracing PUBLIC
    ${CMAKE_SOURCE_DIR}/include/projectHead
    ${CMAKE_SOURCE_DIR}/include/thirdparty
)






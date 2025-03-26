file(GLOB_RECURSE ENGINE_SOURCES
    "source/core/*.cpp"
    "source/resource/*.cpp"
    "source/scene/*.cpp"
    "source/rendering/*.cpp"
)

add_library(engine_lib STATIC)

target_sources(engine_lib PRIVATE ${ENGINE_SOURCES})

# message(STATUS "Engibe sources: ${ENGINE_SOURCES}")

# set(ENGINE_SOURCES
#     source/core/ray.cpp
#     source/resource/object.cpp
#     source/scene/bounding_box.cpp
#     source/scene/bounding_tree.cpp
#     source/scene/scene_object.cpp
#     source/scene/scene.cpp
#     source/rendering/render.cpp
# )

target_include_directories(engine_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party"
)


file(GLOB_RECURSE ENGINE_SOURCES
    "source/platform/normal_system_config.cpp"
    "source/core/*.cpp"
    "source/resource/*.cpp"
    "source/scene/*.cpp"
    "source/rendering/*.cpp"
    "source/tools/*.cpp"
)

if (WIN32)
    message(STATUS "Building for windows")
    file(GLOB_RECURSE PLATFORM_SOURCES
        "source/platform/windows/platform_utils.cpp"
    )
elseif(UNIX AND NOT APPLE)
    message(STATUS "Building for linux")
    file(GLOB_RECURSE PLATFORM_SOURCES
        "source/platform/linux/platform_utils.cpp"
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

add_library(engine_lib STATIC)

target_sources(engine_lib PRIVATE ${ENGINE_SOURCES} ${PLATFORM_SOURCES})

find_package(Threads REQUIRED)

target_link_libraries(engine_lib PRIVATE Threads::Threads)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../"
)


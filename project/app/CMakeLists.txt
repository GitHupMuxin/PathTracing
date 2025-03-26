add_executable(PathTracing)

target_sources(PathTracing PRIVATE
    source/main.cpp
)

target_link_libraries(PathTracing PRIVATE engine_lib)

add_custom_command(TARGET PathTracing POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets" 
    $<TARGET_FILE_DIR:PathTracing>/assets
)
add_executable(PathTracing)

target_sources(PathTracing PRIVATE
    source/main.cpp
)
target_link_libraries(PathTracing PRIVATE engine_lib)

set(ASSETS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_TARGET_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

file(GLOB_RECURSE ASSET_FILES
    "${ASSETS_SOURCE_DIR}/*"
)

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/assets_copied.flag"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_TARGET_DIR}" 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${ASSETS_SOURCE_DIR}"
    "${ASSETS_TARGET_DIR}"
    DEPENDS ${ASSETS_FILE}
    COMMENT "Copying assets to build folder"
    VERBATIM
)

add_custom_target(CopyAssets ALL 
    DEPENDS "${CMAKE_BINARY_DIR}/assets_copied.flag"
)


set(OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/out")
set(OUT_MARKER "${CMAKE_BINARY_DIR}/out_dir_cread.txt")
add_custom_command(
    OUTPUT ${OUT_MARKER}
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${OUT_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E touch "${OUT_MARKER}"
    COMMENT "Creating directory: out"
    VERBATIM
)

add_custom_target(CreadOutDir
    DEPENDS ${OUT_MARKER}
)

add_dependencies(PathTracing CopyAssets CreadOutDir) 





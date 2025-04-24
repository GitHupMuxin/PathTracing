add_executable(PathTracing)

target_sources(PathTracing PRIVATE
    source/main.cpp
)

target_link_libraries(PathTracing PRIVATE engine_lib)

set(ASSETS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_TARGET_DIR "$<TARGET_FILE_DIR:PathTracing>/assets")

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/dummy.txt"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${ASSETS_TARGET_DIR}" 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${ASSETS_SOURCE_DIR}"
    "${ASSETS_TARGET_DIR}"
    COMMENT "Copying assets to assets(build) folder"
    VERBATIM
)

add_custom_target(CopyAssets ALL 
    DEPENDS "${CMAKE_BINARY_DIR}/dummy.txt"
)

set(OUT_DIR "$<TARGET_FILE_DIR:PathTracing>/out}")
set(OUT_MARKER "${OUR_DIR}/.dir_created")

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





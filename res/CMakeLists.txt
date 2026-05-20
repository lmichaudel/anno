file(GLOB_RECURSE SHADERS "shaders/*.sc")

set(SHADER_INCLUDE ${CMAKE_SOURCE_DIR}/lib/bgfx.cmake/bgfx/examples/common ${CMAKE_SOURCE_DIR}/lib/bgfx.cmake/bgfx/src)
foreach (SHADER_FILE IN LISTS SHADERS)
    get_filename_component(FILE_NAME ${SHADER_FILE} NAME_WE)
    get_filename_component(DIR_NAME ${SHADER_FILE} DIRECTORY)
    get_filename_component(EXTENSION ${SHADER_FILE} EXT)

    if (EXTENSION STREQUAL ".vs.sc")
        set(TYPE "VERTEX")
    elseif (EXTENSION STREQUAL ".fs.sc")
        set(TYPE "FRAGMENT")
    else ()
        continue()
    endif ()

    bgfx_compile_shaders(
            TYPE ${TYPE}
            SHADERS ${SHADER_FILE}
            VARYING_DEF ${DIR_NAME}/varying.def.sc
            OUT_FILES_VAR ARTEFACTS
            OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/shaders/${FILE_NAME}
            INCLUDE_DIRS ${SHADER_INCLUDE}
    )

    bgfx_compile_shaders(
            TYPE ${TYPE}
            SHADERS ${SHADER_FILE}
            VARYING_DEF ${DIR_NAME}/varying.def.sc
            OUT_FILES_VAR ARTEFACTS
            OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated/shaders/${FILE_NAME}
            INCLUDE_DIRS ${SHADER_INCLUDE}
    )

    list(APPEND SHADER_ARTEFACTS ${ARTEFACTS})
endforeach ()

add_custom_target(shaders ALL
        DEPENDS ${SHADER_ARTEFACTS}
)

add_custom_target(assets ALL)
add_dependencies(assets shaders)

add_custom_command(TARGET assets POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
        ${CMAKE_CURRENT_BINARY_DIR}/generated
    COMMENT "Copying generated folder to build directory."
)

add_custom_command(TARGET assets POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/textures
        ${CMAKE_CURRENT_BINARY_DIR}/textures
    COMMENT "Copying textures to build directory."
)
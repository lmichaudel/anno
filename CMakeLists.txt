cmake_minimum_required(VERSION 3.29)
project(anno)

add_subdirectory(lib)

include_directories(src)
include_directories(lib/entt/src)
include_directories(res/generated)

set(CMAKE_CXX_STANDARD 20)

set(SHADER_INCLUDE ${CMAKE_SOURCE_DIR}/lib/bgfx.cmake/bgfx/examples/common ${CMAKE_SOURCE_DIR}/lib/bgfx.cmake/bgfx/src)
bgfx_compile_shaders(
        TYPE VERTEX
        SHADERS ${CMAKE_SOURCE_DIR}/res/shaders/cube/cube.vs.sc
        VARYING_DEF ${CMAKE_SOURCE_DIR}/res/shaders/cube/varying.def.sc
        OUTPUT_DIR ${CMAKE_SOURCE_DIR}/res/generated/shaders
        INCLUDE_DIRS ${SHADER_INCLUDE}
)
bgfx_compile_shaders(
        TYPE FRAGMENT
        SHADERS ${CMAKE_SOURCE_DIR}/res/shaders/cube/cube.fs.sc
        VARYING_DEF ${CMAKE_SOURCE_DIR}/res/shaders/cube/varying.def.sc
        OUTPUT_DIR ${CMAKE_SOURCE_DIR}/res/generated/shaders
        INCLUDE_DIRS ${SHADER_INCLUDE}
)
bgfx_compile_shaders(
        TYPE VERTEX
        SHADERS ${CMAKE_SOURCE_DIR}/res/shaders/upscale/upscale.vs.sc
        VARYING_DEF ${CMAKE_SOURCE_DIR}/res/shaders/upscale/varying.def.sc
        OUTPUT_DIR ${CMAKE_SOURCE_DIR}/res/generated/shaders
        INCLUDE_DIRS ${SHADER_INCLUDE}
)
bgfx_compile_shaders(
        TYPE FRAGMENT
        SHADERS ${CMAKE_SOURCE_DIR}/res/shaders/upscale/upscale.fs.sc
        VARYING_DEF ${CMAKE_SOURCE_DIR}/res/shaders/upscale/varying.def.sc
        OUTPUT_DIR ${CMAKE_SOURCE_DIR}/res/generated/shaders
        INCLUDE_DIRS ${SHADER_INCLUDE}
)

add_executable(anno
        src/camera/camera.cpp

        src/core/global.cpp

        src/gfx/framebuffer.cpp
        src/gfx/framebuffer.hpp
        src/gfx/mesh.cpp
        src/gfx/mesh.hpp
        src/gfx/primitive.hpp
        src/gfx/program.cpp
        src/gfx/program.hpp
        src/gfx/renderer.cpp
        src/gfx/renderer.hpp
        src/gfx/texture.cpp
        src/gfx/texture.hpp
        src/gfx/uniform.cpp
        src/gfx/uniform.hpp
        src/gfx/vertex.hpp
        src/gfx/imgui/imgui_impl_bgfx.cpp
        src/gfx/imgui/imgui_impl_bgfx.h
        src/gfx/imgui/vs_ocornut_imgui.bin.h
        src/gfx/imgui/fs_ocornut_imgui.bin.h

        src/log/log.cpp

        src/platform/window.cpp

        src/main.cpp
        src/state.cpp

        res/shaders/cube/cube.vs.sc
        res/shaders/cube/cube.fs.sc
        res/shaders/upscale/upscale.vs.sc
        res/shaders/upscale/upscale.fs.sc
)

target_link_libraries(anno glfw bgfx bx bimg glm imgui)

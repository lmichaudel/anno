cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0175 OLD)

SET(BGFX_BUILD_EXAMPLES ON)
SET(BGFX_BUILD_TOOLS_SHADER ON)
add_subdirectory(bgfx.cmake)

add_library(imgui
        imgui/imconfig.h
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h

        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_glfw.h

        imgui/misc/cpp/imgui_stdlib.cpp
        imgui/misc/cpp/imgui_stdlib.h
)
target_include_directories(imgui PUBLIC imgui/)

set(GLM_TEST_ENABLE OFF CACHE INTERNAL "")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS OFF CACHE INTERNAL "")
add_subdirectory(glm)

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")
add_subdirectory(glfw)
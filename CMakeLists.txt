cmake_minimum_required(VERSION 3.29)
project(anno)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)
add_subdirectory(res)

include_directories(src)
include_directories(lib/entt/src)

file(GLOB_RECURSE HEADERS "src/*.hpp")
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(anno ${HEADERS} ${SOURCES})

target_link_libraries(anno glfw bgfx bx bimg glm imgui)
add_dependencies(anno assets)

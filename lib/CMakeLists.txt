cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0175 OLD)

SET(BGFX_BUILD_EXAMPLES OFF)
SET(BGFX_BUILD_TOOLS_SHADER ON)

add_subdirectory(bgfx.cmake)
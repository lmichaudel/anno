#pragma once

#include <format>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

// -- Float Vectors --
template <>
struct std::formatter<glm::vec2> : std::formatter<std::string> {
    auto format(const glm::vec2& v, std::format_context& ctx) const {
      return std::format_to(ctx.out(), "({}, {})", v.x, v.y);
    }
};

template <>
struct std::formatter<glm::vec3> : std::formatter<std::string> {
    auto format(const glm::vec3& v, std::format_context& ctx) const {
      return std::format_to(ctx.out(), "({}, {}, {})", v.x, v.y, v.z);
    }
};

template <>
struct std::formatter<glm::vec4> : std::formatter<std::string> {
    auto format(const glm::vec4& v, std::format_context& ctx) const {
      return std::format_to(ctx.out(), "({}, {}, {}, {})", v.x, v.y, v.z, v.w);
    }
};

// -- Integer Vectors --
template <>
struct std::formatter<glm::ivec2> : std::formatter<std::string> {
    auto format(const glm::ivec2& v, std::format_context& ctx) const {
      return std::format_to(ctx.out(), "({}, {})", v.x, v.y);
    }
};

template <>
struct std::formatter<glm::ivec3> : std::formatter<std::string> {
    auto format(const glm::ivec3& v, std::format_context& ctx) const {
      return std::format_to(ctx.out(), "({}, {}, {})", v.x, v.y, v.z);
    }
};

template <>
struct std::formatter<glm::ivec4> : std::formatter<std::string> {
    auto format(const glm::ivec4& v, std::format_context& ctx) const {
      return std::format_to(ctx.out(), "({}, {}, {}, {})", v.x, v.y, v.z, v.w);
    }
};

// -- Matrices --
// GLM matrices are column-major by default: mat[col][row]

template <>
struct std::formatter<glm::mat3> : std::formatter<std::string> {
    auto format(const glm::mat3& m, std::format_context& ctx) const {
      return std::format_to(ctx.out(),
                            "[({}, {}, {}), "
                            "({}, {}, {}), "
                            "({}, {}, {})]",
                            m[0][0], m[0][1], m[0][2],
                            m[1][0], m[1][1], m[1][2],
                            m[2][0], m[2][1], m[2][2]);
    }
};

template <>
struct std::formatter<glm::mat4> : std::formatter<std::string> {
    auto format(const glm::mat4& m, std::format_context& ctx) const {
      return std::format_to(ctx.out(),
                            "[({}, {}, {}, {}), "
                            "({}, {}, {}, {}), "
                            "({}, {}, {}, {}), "
                            "({}, {}, {}, {})]",
                            m[0][0], m[0][1], m[0][2], m[0][3],
                            m[1][0], m[1][1], m[1][2], m[1][3],
                            m[2][0], m[2][1], m[2][2], m[2][3],
                            m[3][0], m[3][1], m[3][2], m[3][3]);
    }
};
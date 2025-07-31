#pragma once

#include "gfx/vertex.hpp"

#include <array>

namespace lm::primitives {
  static constexpr auto cube_vertices = std::array{
      Vertex{{-1, -1, 1}, {0, 0, 1}, {0.0f, 0.0f}},
      Vertex{{1, -1, 1}, {0, 0, 1}, {1.0f, 0.0f}},
      Vertex{{1, 1, 1}, {0, 0, 1}, {1.0f, 1.0f}},
      Vertex{{-1, 1, 1}, {0, 0, 1}, {0.0f, 1.0f}},
      Vertex{{-1, -1, -1}, {0, 0, -1}, {1.0f, 0.0f}},
      Vertex{{-1, 1, -1}, {0, 0, -1}, {1.0f, 1.0f}},
      Vertex{{1, 1, -1}, {0, 0, -1}, {0.0f, 1.0f}},
      Vertex{{1, -1, -1}, {0, 0, -1}, {0.0f, 0.0f}},
      Vertex{{-1, 1, -1}, {0, 1, 0}, {0.0f, 1.0f}},
      Vertex{{-1, 1, 1}, {0, 1, 0}, {0.0f, 0.0f}},
      Vertex{{1, 1, 1}, {0, 1, 0}, {1.0f, 0.0f}},
      Vertex{{1, 1, -1}, {0, 1, 0}, {1.0f, 1.0f}},
      Vertex{{-1, -1, -1}, {0, -1, 0}, {0.0f, 0.0f}},
      Vertex{{1, -1, -1}, {0, -1, 0}, {1.0f, 0.0f}},
      Vertex{{1, -1, 1}, {0, -1, 0}, {1.0f, 1.0f}},
      Vertex{{-1, -1, 1}, {0, -1, 0}, {0.0f, 1.0f}},
      Vertex{{1, -1, -1}, {1, 0, 0}, {0.0f, 0.0f}},
      Vertex{{1, 1, -1}, {1, 0, 0}, {0.0f, 1.0f}},
      Vertex{{1, 1, 1}, {1, 0, 0}, {1.0f, 1.0f}},
      Vertex{{1, -1, 1}, {1, 0, 0}, {1.0f, 0.0f}},
      Vertex{{-1, -1, -1}, {-1, 0, 0}, {1.0f, 0.0f}},
      Vertex{{-1, -1, 1}, {-1, 0, 0}, {0.0f, 0.0f}},
      Vertex{{-1, 1, 1}, {-1, 0, 0}, {0.0f, 1.0f}},
      Vertex{{-1, 1, -1}, {-1, 0, 0}, {1.0f, 1.0f}}};

  static constexpr auto cube_indices = std::array<Indice, 36>{
      0, 1, 2, 2, 3, 0,
      4, 5, 6, 6, 7, 4,
      8, 9, 10, 10, 11, 8,
      12, 13, 14, 14, 15, 12,
      16, 17, 18, 18, 19, 16,
      20, 21, 22, 22, 23, 20};

  static constexpr auto quad_vertices = std::array{
      Vertex{{-1.0f, -1.0f, 0.0f}, {0, 0, 1}, {0.0f, 0.0f}},
      Vertex{{1.0f, -1.0f, 0.0f}, {0, 0, 1}, {1.0f, 0.0f}},
      Vertex{{1.0f, 1.0f, 0.0f}, {0, 0, 1}, {1.0f, 1.0f}},
      Vertex{{-1.0f, 1.0f, 0.0f}, {0, 0, 1}, {0.0f, 1.0f}},
  };

  static constexpr auto quad_indices = std::array<Indice, 6>{
      0, 1, 2,
      2, 3, 0};

  static constexpr auto plane_vertices = std::array{
      Vertex{{-1.0f, 0.0f, -1.0f}, {0, 1, 0}, {0.0f, 0.0f}},
      Vertex{{1.0f, 0.0f, -1.0f}, {0, 1, 0}, {1.0f, 0.0f}},
      Vertex{{1.0f, 0.0f, 1.0f}, {0, 1, 0}, {1.0f, 1.0f}},
      Vertex{{-1.0f, 0.0f, 1.0f}, {0, 1, 0}, {0.0f, 1.0f}},
  };

  static constexpr auto plane_indices = std::array<Indice, 6>{
      0, 1, 2,
      2, 3, 0};

  static constexpr auto cylinder_vertices = std::array{
      Vertex{{0.0f, -1.0f, 0.0f}, {0, -1, 0}, {0.5f, 0.5f}}, // bottom center
      Vertex{{0.0f, 1.0f, 0.0f}, {0, 1, 0}, {0.5f, 0.5f}},   // top center

      // bottom ring
      Vertex{{1.0f, -1.0f, 0.0f}, {1, 0, 0}, {1.0f, 0.5f}},
      Vertex{{0.707f, -1.0f, 0.707f}, {0.707f, 0, 0.707f}, {0.85f, 0.85f}},
      Vertex{{0.0f, -1.0f, 1.0f}, {0, 0, 1}, {0.5f, 1.0f}},
      Vertex{{-0.707f, -1.0f, 0.707f}, {-0.707f, 0, 0.707f}, {0.15f, 0.85f}},
      Vertex{{-1.0f, -1.0f, 0.0f}, {-1, 0, 0}, {0.0f, 0.5f}},
      Vertex{{-0.707f, -1.0f, -0.707f}, {-0.707f, 0, -0.707f}, {0.15f, 0.15f}},
      Vertex{{0.0f, -1.0f, -1.0f}, {0, 0, -1}, {0.5f, 0.0f}},
      Vertex{{0.707f, -1.0f, -0.707f}, {0.707f, 0, -0.707f}, {0.85f, 0.15f}},

      // top ring (same positions but y = 1.0f)
      Vertex{{1.0f, 1.0f, 0.0f}, {1, 0, 0}, {1.0f, 0.5f}},
      Vertex{{0.707f, 1.0f, 0.707f}, {0.707f, 0, 0.707f}, {0.85f, 0.85f}},
      Vertex{{0.0f, 1.0f, 1.0f}, {0, 0, 1}, {0.5f, 1.0f}},
      Vertex{{-0.707f, 1.0f, 0.707f}, {-0.707f, 0, 0.707f}, {0.15f, 0.85f}},
      Vertex{{-1.0f, 1.0f, 0.0f}, {-1, 0, 0}, {0.0f, 0.5f}},
      Vertex{{-0.707f, 1.0f, -0.707f}, {-0.707f, 0, -0.707f}, {0.15f, 0.15f}},
      Vertex{{0.0f, 1.0f, -1.0f}, {0, 0, -1}, {0.5f, 0.0f}},
      Vertex{{0.707f, 1.0f, -0.707f}, {0.707f, 0, -0.707f}, {0.85f, 0.15f}},
  };

  static constexpr auto cylinder_indices = std::array<Indice, 96>{
      // sides (quads as two triangles)
      2,
      12,
      13,
      13,
      3,
      2,
      3,
      13,
      14,
      14,
      4,
      3,
      4,
      14,
      15,
      15,
      5,
      4,
      5,
      15,
      16,
      16,
      6,
      5,
      6,
      16,
      17,
      17,
      7,
      6,
      7,
      17,
      18,
      18,
      8,
      7,
      8,
      18,
      19,
      19,
      9,
      8,
      9,
      19,
      12,
      12,
      2,
      9,

      // bottom fan
      0,
      2,
      3,
      0,
      3,
      4,
      0,
      4,
      5,
      0,
      5,
      6,
      0,
      6,
      7,
      0,
      7,
      8,
      0,
      8,
      9,
      0,
      9,
      2,

      // top fan
      1,
      13,
      12,
      1,
      14,
      13,
      1,
      15,
      14,
      1,
      16,
      15,
      1,
      17,
      16,
      1,
      18,
      17,
      1,
      19,
      18,
      1,
      12,
      19,
  };

} // namespace lm::primitives
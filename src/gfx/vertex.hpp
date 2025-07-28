#pragma once

#include <bgfx/bgfx.h>

struct Vertex {
  float position[3];
  float normal[3];
  float uv[2];

  static bgfx::VertexLayout& layout() {
    static bool is_layout_initialized = false;
    static bgfx::VertexLayout layout;

    if (!is_layout_initialized) {
      is_layout_initialized = true;
      layout
          .begin()
          .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
          .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
          .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
          .end();
    }

    return layout;
  }
};

using Indice = uint16_t;


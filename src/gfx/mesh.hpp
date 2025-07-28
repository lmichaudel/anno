#pragma once

#include "gfx/program.hpp"
#include "gfx/vertex.hpp"

#include <span>

namespace lm {
  class Mesh {
    bgfx::VertexBufferHandle vbh{bgfx::kInvalidHandle};
    bgfx::IndexBufferHandle ibh{bgfx::kInvalidHandle};

    public:
      Mesh() = default;
      ~Mesh();
      Mesh(std::span<const Vertex> vertices, std::span<const Indice> indices);

      Mesh(const Mesh&) = delete;
      Mesh& operator=(const Mesh&) = delete;

      Mesh(Mesh&& other) noexcept;
      Mesh& operator=(Mesh&& other) noexcept;

      void render(bgfx::ViewId view_id, const Program& program) const;
  };
}
#pragma once

#include "gfx/vertex.hpp"

#include <span>

namespace lm {
  class Mesh {
      bgfx::VertexBufferHandle m_vbh{bgfx::kInvalidHandle};
      bgfx::IndexBufferHandle m_ibh{bgfx::kInvalidHandle};

    public:
      Mesh() = default;
      ~Mesh();
      Mesh(std::span<const Vertex> vertices, std::span<const Indice> indices);

      Mesh(const Mesh&) = delete;
      Mesh& operator=(const Mesh&) = delete;

      Mesh(Mesh&& other) noexcept;
      Mesh& operator=(Mesh&& other) noexcept;

      void bind() const;
  };
} // namespace lm
#include "gfx/mesh.hpp"

#include <cassert>

namespace lm {
  Mesh::Mesh(std::span<const Vertex> vertices, std::span<const Indice> indices) {
    assert(!vertices.empty());
    assert(!indices.empty());

    m_vbh = bgfx::createVertexBuffer(bgfx::copy(&vertices[0], sizeof(Vertex) * vertices.size()), Vertex::layout());
    m_ibh = bgfx::createIndexBuffer(bgfx::copy(&indices[0], sizeof(Indice) * indices.size()));
  }

  Mesh::~Mesh() {
    if (bgfx::isValid(m_vbh)) {
      bgfx::destroy(m_vbh);
    }

    if (bgfx::isValid(m_ibh)) {
      bgfx::destroy(m_ibh);
    }
  }

  void Mesh::bind() const {
    assert(bgfx::isValid(m_vbh));
    assert(bgfx::isValid(m_ibh));

    bgfx::setVertexBuffer(0, m_vbh);
    bgfx::setIndexBuffer(m_ibh);
  }

  Mesh::Mesh(Mesh&& other) noexcept : m_vbh(other.m_vbh), m_ibh(other.m_ibh) {
    other.m_ibh = {bgfx::kInvalidHandle};
    other.m_vbh = {bgfx::kInvalidHandle};
  }

  Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
      if (bgfx::isValid(m_vbh)) {
        bgfx::destroy(m_vbh);
      }
      if (bgfx::isValid(m_ibh)) {
        bgfx::destroy(m_ibh);
      }

      m_vbh = other.m_vbh;
      m_ibh = other.m_ibh;

      other.m_vbh = {bgfx::kInvalidHandle};
      other.m_ibh = {bgfx::kInvalidHandle};
    }
    return *this;
  }
} // namespace lm
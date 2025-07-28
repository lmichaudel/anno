#include "gfx/mesh.hpp"

#include "log/log.hpp"

#include <cassert>

namespace lm {
  Mesh::Mesh(std::span<const Vertex> vertices, std::span<const Indice> indices) {
    assert(!vertices.empty());
    assert(!indices.empty());

    vbh = bgfx::createVertexBuffer(bgfx::copy(&vertices[0], sizeof(Vertex) * vertices.size()), Vertex::layout());
    ibh = bgfx::createIndexBuffer(bgfx::copy(&indices[0], sizeof(Indice) * indices.size()));
  }

  Mesh::~Mesh() {
    if (bgfx::isValid(vbh)) {
      LOG_DEBUG("Destroying vertex buffer {}.", vbh.idx);
      bgfx::destroy(vbh);
    }

    if (bgfx::isValid(ibh)) {
      LOG_DEBUG("Destroying index buffer {}.", ibh.idx);
      bgfx::destroy(ibh);
    }
  }

  void Mesh::render(bgfx::ViewId view_id, const Program& program) const {
    assert(bgfx::isValid(vbh));
    assert(bgfx::isValid(ibh));

    bgfx::setVertexBuffer(0, vbh);
    bgfx::setIndexBuffer(ibh);
    bgfx::submit(view_id, program);
  }

  Mesh::Mesh(Mesh&& other) noexcept :
    vbh(other.vbh), ibh(other.ibh) {
    other.ibh = {bgfx::kInvalidHandle};
    other.vbh = {bgfx::kInvalidHandle};
  }

  Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
      if (bgfx::isValid(vbh)) {
        bgfx::destroy(vbh);
      }
      if (bgfx::isValid(ibh)) {
        bgfx::destroy(ibh);
      }

      vbh = other.vbh;
      ibh = other.ibh;

      other.vbh = {bgfx::kInvalidHandle};
      other.ibh = {bgfx::kInvalidHandle};
    }
    return *this;
  }
}
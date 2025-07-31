#pragma once

#include "gfx/mesh.hpp"

#include <memory>

namespace lm {
  struct MeshRenderer {
      std::shared_ptr<Mesh> mesh;
  };
} // namespace lm
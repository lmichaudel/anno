#include "core/global.hpp"

#include "gfx/renderer.hpp"
#include "glm/trigonometric.hpp"
#include "platform/window.hpp"

namespace lm {
  Global::Global() {
    window = std::make_unique<Window>();
    renderer = std::make_unique<Renderer>(*window);
    camera = std::make_unique<Camera>();
  }
} // namespace lm
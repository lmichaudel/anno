#include "core/global.hpp"

#include "camera/camera.hpp"
#include "gfx/renderer.hpp"
#include "platform/input.hpp"
#include "platform/window.hpp"

namespace lm {
  Global::Global() {
    input = std::make_unique<Input>();
    window = std::make_unique<Window>();
    renderer = std::make_unique<Renderer>();
    camera = std::make_unique<Camera>();
  }
} // namespace lm
#pragma once

#include "core/ecs.hpp"

#include "camera/camera.hpp"
#include "gfx/renderer.hpp"
#include "platform/input.hpp"
#include "platform/window.hpp"

namespace lm {
  struct Engine {
      // entt::registry ecs;
      Input input{};
      Window window{input};
      Camera camera{};
      Renderer renderer{window, camera};

      ~Engine() = default;
  };
} // namespace lm
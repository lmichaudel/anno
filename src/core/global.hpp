#pragma once

#include "camera/camera.hpp"
#include "core/ecs.hpp"

#include <memory>

namespace lm {
  class Window;
  class Renderer;

  struct Global {
      entt::registry ecs;
      std::unique_ptr<Window> window{nullptr};
      std::unique_ptr<Renderer> renderer{nullptr};
      std::unique_ptr<Camera> camera{nullptr};

      Global();
      ~Global() = default;
  };

  extern std::unique_ptr<Global> global;
} // namespace lm

#pragma once

#include "camera/camera.hpp"

#include <memory>

namespace lm {
  class Window;
  class Renderer;

  struct Global {
    std::unique_ptr<Window> window{nullptr};
    std::unique_ptr<Renderer> renderer{nullptr};
    std::unique_ptr<Camera> camera{nullptr};

    Global();
    ~Global() = default;
  };

  extern std::unique_ptr<Global> global;
}

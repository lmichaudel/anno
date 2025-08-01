#pragma once

namespace lm::CONSTANTS {
  constexpr int RENDER_WIDTH = 320 + 2;
  constexpr int RENDER_HEIGHT = 180 + 2;
  constexpr float ASPECT = static_cast<float>(RENDER_WIDTH) / static_cast<float>(RENDER_HEIGHT);

  constexpr int WINDOW_DEFAULT_WIDTH = 1280;
  constexpr int WINDOW_DEFAULT_HEIGHT = 720;
} // namespace lm::CONSTANTS

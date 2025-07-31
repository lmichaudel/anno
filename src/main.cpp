#include "main.hpp"

#include "core/global.hpp"
#include "log/log.hpp"
#include "state.hpp"

#include <chrono>

namespace lm {
  std::unique_ptr<Global> global;
}

using namespace lm;

using Clock = std::chrono::high_resolution_clock;
using TimeStamp = std::chrono::time_point<Clock>;

static constexpr float FIXED_DT = 1.0 / 60.0;

int main() {
  global = std::make_unique<Global>();
  LOG_DEBUG("Game started.");

  {
    GameState state{};

    TimeStamp previous_stamp = Clock::now();
    float accumulator = 0.0;

    while (!global->window->should_close()) {
      TimeStamp current_stamp = Clock::now();
      const float dt = std::chrono::duration<float>(current_stamp - previous_stamp).count();
      previous_stamp = current_stamp;
      accumulator += dt;

      global->window->poll();

      while (accumulator >= FIXED_DT) {
        state.tick(FIXED_DT);
        accumulator -= FIXED_DT;
      }

      state.update(dt);
      state.draw(dt);
    }
  }

  // We need to do this or X11 will crash (we need to clean glfw before main exits).
  global.reset(nullptr);
  LOG_DEBUG("Cleaned up.");
  return 0;
}

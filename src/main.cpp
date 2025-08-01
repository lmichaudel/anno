#include "game.hpp"
#include "log/log.hpp"

#include <chrono>

using namespace lm;

using Clock = std::chrono::high_resolution_clock;
using TimeStamp = std::chrono::time_point<Clock>;

static constexpr float FIXED_DT = 1.0 / 60.0;

int main() {
  auto engine = Engine{};
  auto game = Game{engine};
  game.init();
  LOG_DEBUG("Game started.");

  TimeStamp previous_stamp = Clock::now();
  float accumulator = 0.0;
  while (!engine.window.should_close()) {
    TimeStamp current_stamp = Clock::now();
    const float dt = std::chrono::duration<float>(current_stamp - previous_stamp).count();
    previous_stamp = current_stamp;
    accumulator += dt;

    engine.window.poll();

    while (accumulator >= FIXED_DT) {
      game.tick(FIXED_DT);
      accumulator -= FIXED_DT;
    }

    game.update(dt);
    game.render(dt);
  }

  LOG_DEBUG("Cleaned up.");
  return 0;
}

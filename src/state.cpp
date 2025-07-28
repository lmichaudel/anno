#include "state.hpp"

#include "core/global.hpp"

#include <bx/math.h>

namespace lm {

  void GameState::update(double dt) {
    ax += dt * 0.5f;
    ay += dt * 0.5f;
    bx::mtxRotateXY(mtxModel, ax, ay);
  }

  void GameState::tick(double dt) {

  }

  void GameState::draw(double dt) {
    const bgfx::ViewId view_id = global->renderer->begin_frame();

    global->camera->bind();

    bgfx::setTransform(mtxModel);

    s_texture.bind_texture(texture);
    u_light_dir.set_data(lightDir);

    cube.render(view_id, program);
    global->renderer->end_frame();

    const bgfx::Stats* stats = bgfx::getStats();
    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(0, 0, 0x0f, "Pixel Art 3D Cube Demo");
    bgfx::dbgTextPrintf(0, 1, 0x0f, "Rendering at 320x180, upscaled to %dx%d.", global->window->width(), global->window->height());
    bgfx::dbgTextPrintf(0, 2, 0x0f, "%i fps (CPU:%.2fms GPU:%.2fms)", static_cast<int>(1.0 / dt), 1000.0 * dt, static_cast<float>(stats->gpuTimeEnd - stats->gpuTimeBegin) * 1000.0 / stats->gpuTimerFreq);
    bgfx::frame();
  }
}

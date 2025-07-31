#include "state.hpp"

#include "core/global.hpp"
#include "glm/gtx/transform.hpp"
#include "log/log.hpp"

#include <imgui.h>

#include <bx/math.h>

namespace lm {

  void GameState::update(float dt) {
    Camera* cam = global->camera.get();

    if (global->window->was_key_pressed(GLFW_KEY_E)) {
      cam->set_pivot_position(cam->get_pivot_position() + cam->right() * cam->unit_pixel() / 2.0f);
    }

    if (global->window->was_key_pressed(GLFW_KEY_Q)) {
      cam->smooth = !cam->smooth;
      LOG_DEBUG("cam smooth: {}.", cam->smooth);
    }

    glm::vec3 panning{0.0f};
    if (global->window->is_key_down(GLFW_KEY_A)) {
      panning -= cam->right();
    }
    if (global->window->is_key_down(GLFW_KEY_D)) {
      panning += cam->right();
    }
    if (global->window->is_key_down(GLFW_KEY_W)) {
      panning += cam->up();
    }
    if (global->window->is_key_down(GLFW_KEY_S)) {
      panning -= cam->up();
    }

    if (panning != glm::vec3{0.0f}) {
      constexpr auto speed = 5.f;
      cam->set_pivot_position(cam->get_pivot_position() + glm::normalize(panning) * speed * dt);
    }
  }

  void GameState::tick(const float dt) {
  }

  void GameState::draw(const float dt) {
    const bgfx::ViewId view_id = global->renderer->begin_frame();

    global->camera->bind();

    float model[16];
    bx::mtxScale(model, 10.f, 1.f, 10.f);
    bgfx::setTransform(model);
    s_texture.bind_texture(rock);
    u_light_dir.set_data(lightDir);
    plane.render(view_id, program);

    auto cube_model = glm::mat4(1.0f);
    cube_model = glm::translate(cube_model, glm::vec3(0.0, 1.0f, 0.0f));
    bgfx::setTransform(&cube_model[0][0]);
    s_texture.bind_texture(texture);
    u_light_dir.set_data(lightDir);
    cube.render(view_id, program);

    // ImGui::ShowDemoWindow();

    global->renderer->end_frame();

    const bgfx::Stats* stats = bgfx::getStats();
    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(0, 0, 0x0f, "3D Pixel art");
    bgfx::dbgTextPrintf(0, 1, 0x0f, "Rendering at 320x180, upscaled to %dx%d.", global->window->width(), global->window->height());
    bgfx::dbgTextPrintf(0, 2, 0x0f, "%i fps (CPU:%.2fms GPU:%.2fms)", static_cast<int>(1.0 / dt), 1000.0 * dt, static_cast<float>(stats->gpuTimeEnd - stats->gpuTimeBegin) * 1000.0 / stats->gpuTimerFreq);
    bgfx::frame();
  }
} // namespace lm

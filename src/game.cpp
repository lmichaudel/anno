#include "game.hpp"

#include "log/log.hpp"

#include "gfx/primitive.hpp"

void lm::Game::init() {
  cube = {primitives::cube_vertices, primitives::cube_indices};
  plane = {primitives::plane_vertices, primitives::plane_indices};
}

void lm::Game::update(const float dt) {
  if (engine.input.is_button_down(GLFW_MOUSE_BUTTON_RIGHT)) {
    engine.camera.set_rotation(engine.camera.get_rotation() - engine.input.get_cursor_delta().x * dt * 100.0f);
  }

  glm::vec3 panning{0.0f};
  if (engine.input.is_key_down(GLFW_KEY_A)) {
    panning -= engine.camera.right();
  }
  if (engine.input.is_key_down(GLFW_KEY_D)) {
    panning += engine.camera.right();
  }
  if (engine.input.is_key_down(GLFW_KEY_W)) {
    panning += engine.camera.up();
  }
  if (engine.input.is_key_down(GLFW_KEY_S)) {
    panning -= engine.camera.up();
  }

  if (panning != glm::vec3{0.0f}) {
    constexpr auto speed = 5.f;
    engine.camera.set_pivot_position(engine.camera.get_pivot_position() + glm::normalize(panning) * speed * dt);
  }
}

void lm::Game::tick(float dt) {
}

void lm::Game::render(const float dt) const {
  const bgfx::ViewId view_id = engine.renderer.begin_frame();

  engine.camera.bind();

  auto quad_model = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
  bgfx::setTransform(&quad_model[0][0]);
  s_texture.bind_texture(rock);
  engine.renderer.set_id(50);
  u_light_dir.set_data(light_dir);
  plane.render(view_id, program);

  auto cube_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 1.0f, 0.0f));
  bgfx::setTransform(&cube_model[0][0]);
  s_texture.bind_texture(texture);
  u_light_dir.set_data(light_dir);
  engine.renderer.set_id(3567);
  cube.render(view_id, program);

  // ImGui::ShowDemoWindow();

  engine.renderer.end_frame();

  const bgfx::Stats* stats = bgfx::getStats();
  bgfx::dbgTextClear();
  bgfx::dbgTextPrintf(0, 0, 0x0f, "3D Pixel art");
  bgfx::dbgTextPrintf(0, 1, 0x0f, "Rendering at 320x180, upscaled to %dx%d.", engine.window.width(), engine.window.height());
  bgfx::dbgTextPrintf(0, 2, 0x0f, "Picked : %i", engine.renderer.who_is_at(engine.input.get_cursor_position()));
  bgfx::dbgTextPrintf(0, 3, 0x0f, "%i fps (CPU:%.2fms GPU:%.2fms)", static_cast<int>(1.0 / dt), 1000.0 * dt, static_cast<float>(stats->gpuTimeEnd - stats->gpuTimeBegin) * 1000.0 / stats->gpuTimerFreq);
  bgfx::frame();
}
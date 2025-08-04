#include "game.hpp"

#include "imgui.h"
#include "log/log.hpp"

#include "gfx/primitive.hpp"

void lm::Game::init() {
  program = {"model"};
  program.add_sampler("s_albedo", 0);
  program.add_uniform("u_pick_id", bgfx::UniformType::Vec4);

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
  engine.renderer.begin_frame();

  engine.camera.bind();

  const auto quad_model = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));
  program.set_sampler("s_albedo", rock);
  engine.renderer.render_mesh(plane, program, quad_model, 1);

  constexpr auto cube_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 1.0f, 0.0f));
  program.set_sampler("s_albedo", texture);
  engine.renderer.render_mesh(cube, program, cube_model, 2);

  const std::vector light_positions = {glm::vec4(5.0f, 4.0f, 2.0f, 20.0f), glm::vec4(-5.0f, 2.0f, -3.0f, 15.0f)};
  const std::vector light_colors = {glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0.5f, 0.5f, 1.0f)};

  static float acc = 0.0f;
  acc += 40.0f * dt;
  const auto angles = glm::vec3(0.0f, glm::radians(acc), 0.0f);
  const auto rotation_matrix = glm::mat3(glm::quat(angles));

  engine.renderer.render_point_light(glm::normalize(rotation_matrix * glm::vec3(1.0f, 1.0f, 0.0f)) * 8.0f, 30.0f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
  engine.renderer.render_point_light(glm::vec3(-7.0f, 2.0f, -5.0f), 20.0f, 2.0f, glm::vec3(1.0f, 0.25f, 0.25f));

  engine.renderer.render_ambient_light(glm::vec3(1.0f, 1.0f, 1.0f), 0.005f);
  engine.renderer.render_directional_light(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);

  const bgfx::Stats* stats = bgfx::getStats();
  bgfx::dbgTextClear();
  bgfx::dbgTextPrintf(0, 0, 0x0f, "3D Pixel art");
  bgfx::dbgTextPrintf(0, 1, 0x0f, "Rendering at 320x180, upscaled to %dx%d.", engine.window.width(), engine.window.height());
  bgfx::dbgTextPrintf(0, 2, 0x0f, "Picked : %i", engine.renderer.who_is_at(engine.input.get_cursor_position()));
  bgfx::dbgTextPrintf(0, 3, 0x0f, "%i fps (CPU:%.2fms GPU:%.2fms)", static_cast<int>(1.0 / dt), 1000.0 * dt, static_cast<float>(stats->gpuTimeEnd - stats->gpuTimeBegin) * 1000.0 / stats->gpuTimerFreq);

  ImGui::Begin("Demo");
  ImGui::Text("Hello World, I'm ImGUI!");
  ImGui::End();

  engine.renderer.end_frame();
}
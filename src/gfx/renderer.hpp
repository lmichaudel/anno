#pragma once

#include "gfx/framebuffer.hpp"
#include "gfx/mesh.hpp"
#include "gfx/program.hpp"
#include "gfx/uniform.hpp"

#include "camera/camera.hpp"

#include "platform/window.hpp"
#include "util/constants.hpp"

namespace lm {
  struct IRenderable {
      void bind();
  };

  class Renderer {
      static constexpr bgfx::ViewId
          MAIN_VIEW = 0,
          LIGHT_VIEW = 1,
          UPSCALE_VIEW = 3,
          UPLOAD_VIEW = 4; // TODO

      Window& m_window;
      Camera& m_camera;

      Texture m_cpu_side_id_texture;

      // Step 1 : render geometry data to the G-Buffer
      Texture m_gbuffer_position, m_gbuffer_normal, m_gbuffer_albedo, m_gbuffer_data, m_gbuffer_depth;
      Framebuffer m_gbuffer;

      // Step 2 : generate the lit scene using the G-Buffer
      Texture m_main_framebuffer_color;
      Program m_light_program;
      Framebuffer m_main_framebuffer;

      // Step 3 : upscale to the main framebuffer
      Program m_upscale_program;

      // TODO
      uint8_t m_blit_data[CONSTANTS::RENDER_WIDTH * CONSTANTS::RENDER_HEIGHT * 4]{};

      Mesh m_fullscreen_quad;

    public:
      Renderer(Window& window, Camera& camera);
      ~Renderer() = default;

      Renderer(const Renderer&) = delete;
      Renderer& operator=(const Renderer&) = delete;

      void render_mesh(const Mesh& mesh, const Program& program, glm::mat4 model = glm::mat4(1.0f), uint16_t id = 0) const;

      uint16_t who_is_at(glm::ivec2 screen_position) const;

      void begin_frame() const;
      void end_frame();
  };
} // namespace lm
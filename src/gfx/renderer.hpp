#pragma once

#include "gfx/framebuffer.hpp"
#include "gfx/mesh.hpp"
#include "gfx/program.hpp"
#include "gfx/uniform.hpp"

#include "camera/camera.hpp"

#include "platform/window.hpp"
#include "util/constants.hpp"

namespace lm {
  class Renderer {
      static constexpr bgfx::ViewId
          MAIN_VIEW = 0,
          UPSCALE_VIEW = 1,
          UPLOAD_VIEW = 2;

      Window& m_window;
      Camera& m_camera;

      Texture m_framebuffer_color, m_framebuffer_id, m_framebuffer_depth;
      Texture m_cpu_side_id_texture;
      Framebuffer m_framebuffer;

      uint8_t m_blit_data[CONSTANTS::RENDER_WIDTH * CONSTANTS::RENDER_HEIGHT * 4]{};

      Program m_upscale_program;
      Uniform m_low_resolution_sampler;
      Uniform m_texel_error;
      Uniform m_id_uniform;
      Mesh m_fullscreen_quad;

    public:
      Renderer(Window& window, Camera& camera);
      ~Renderer() = default;

      Renderer(const Renderer&) = delete;
      Renderer& operator=(const Renderer&) = delete;

      uint16_t who_is_at(glm::ivec2 screen_position);

      void set_id(uint16_t id) const;
      bgfx::ViewId begin_frame() const;
      void end_frame();
  };
} // namespace lm
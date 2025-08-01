#pragma once

#include "gfx/framebuffer.hpp"
#include "gfx/mesh.hpp"
#include "gfx/program.hpp"
#include "gfx/uniform.hpp"

#include "camera/camera.hpp"

#include "platform/window.hpp"

namespace lm {
  class Renderer {
      static constexpr bgfx::ViewId MAIN_VIEW_ID = 0;
      static constexpr bgfx::ViewId UPSCALE_VIEW_ID = 1;

      Window& m_window;
      Camera& m_camera;

      Framebuffer m_framebuffer;
      Program m_upscale_program;
      Uniform m_low_resolution_sampler;
      Uniform m_texel_error;
      Mesh m_fullscreen_quad;

    public:
      Renderer(Window& window, Camera& camera);
      ~Renderer() = default;

      Renderer(const Renderer&) = delete;
      Renderer& operator=(const Renderer&) = delete;

      bgfx::ViewId begin_frame() const;
      void end_frame() const;
  };
} // namespace lm
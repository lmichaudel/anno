#include "gfx/renderer.hpp"

#include "core/global.hpp"
#include "imgui/imgui_impl_bgfx.h"
#include "log/log.hpp"

#include <backends/imgui_impl_glfw.h>

namespace lm {
  bgfx::ViewId Renderer::begin_frame() const {
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);
    bgfx::setViewName(MAIN_VIEW_ID, "Low resolution pass");
    bgfx::setViewRect(MAIN_VIEW_ID, 0, 0, LOW_RESOLUTION_WIDTH, LOW_RESOLUTION_HEIGHT);
    bgfx::setViewFrameBuffer(MAIN_VIEW_ID, m_framebuffer);
    bgfx::setViewClear(MAIN_VIEW_ID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x101010ff, 1.0f, 0);

    ImGui_Implbgfx_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    return MAIN_VIEW_ID;
  }

  void Renderer::end_frame() const {
    bgfx::setState(BGFX_STATE_WRITE_RGB);
    bgfx::setViewName(UPSCALE_VIEW_ID, "Upscale Pass");
    bgfx::setViewRect(UPSCALE_VIEW_ID, 0, 0, m_window.width(), m_window.height());
    bgfx::setViewFrameBuffer(UPSCALE_VIEW_ID, BGFX_INVALID_HANDLE);
    bgfx::setViewClear(UPSCALE_VIEW_ID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xff0000ff, 1.0f, 0);

    const glm::vec2 shift = global->camera->get_shift();
    // The idea is the map the uv range (0,1) to (ONE_PIXEL, 1.0f- 2.0f*ONE_PIXEL) so we render the (1, 1, 320, 180) cutout
    // We then get a 1 pixel padding around the whole image, that we use with shift
    // texel_errors represents this cutout, it's a rect (x, y, w, h) in uv range instead of pixel range
    constexpr auto UV_ONE_PIXEL = glm::vec2(1.0f / LOW_RESOLUTION_WIDTH, 1.0f / LOW_RESOLUTION_HEIGHT);
    float texel_errors[4] = {UV_ONE_PIXEL.x, UV_ONE_PIXEL.y, 1.0f - (2.0f * UV_ONE_PIXEL.x), 1.0f - (2.0f * UV_ONE_PIXEL.y)};

    if (global->camera->smooth) {
      texel_errors[0] += shift.x;
      texel_errors[1] += shift.y;
    }

    m_texel_error.set_data(&texel_errors[0]);

    m_low_resolution_sampler.bind_texture(m_framebuffer.get_color_texture());
    m_fullscreen_quad.render(UPSCALE_VIEW_ID, m_upscale_program);

    ImGui::Render();
    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
  }
} // namespace lm
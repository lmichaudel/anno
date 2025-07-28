#include "gfx/renderer.hpp"

#include "imgui/imgui_impl_bgfx.h"

#include <backends/imgui_impl_glfw.h>

namespace lm {
  bgfx::ViewId Renderer::begin_frame() const {
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW);
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

    m_low_resolution_sampler.bind_texture(m_framebuffer.get_color_texture());
    m_fullscreen_quad.render(UPSCALE_VIEW_ID, m_upscale_program);

    ImGui::Render();
    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
  }
}
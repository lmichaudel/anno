#include "gfx/renderer.hpp"

#include "maths/maths.hpp"
#include "util/constants.hpp"

#include "backends/imgui_impl_glfw.h"
#include "imgui/imgui_impl_bgfx.h"
#include "log/log.hpp"

namespace lm {

  Renderer::Renderer(Window& window, Camera& camera) : m_window(window), m_camera(camera) {
    m_framebuffer_color = Texture{CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT, bgfx::TextureFormat::RGBA8, 0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT};
    m_framebuffer_id = Texture{CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT, bgfx::TextureFormat::RGBA8, 0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT};
    m_framebuffer_depth = Texture{CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT, bgfx::TextureFormat::D24S8, 0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT};
    m_cpu_side_id_texture = Texture{CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT, bgfx::TextureFormat::RGBA8, 0 | BGFX_TEXTURE_BLIT_DST | BGFX_TEXTURE_READ_BACK | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP};

    m_framebuffer = Framebuffer{std::vector{&m_framebuffer_color, &m_framebuffer_id, &m_framebuffer_depth}};

    m_upscale_program = {"upscale"};
    m_low_resolution_sampler = {"s_albedo", bgfx::UniformType::Enum::Sampler};
    m_texel_error = {"u_texel_error", bgfx::UniformType::Enum::Vec4};
    m_id_uniform = {"u_id", bgfx::UniformType::Vec4};

    m_fullscreen_quad = {
        std::array{
            Vertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            Vertex{{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            Vertex{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            Vertex{{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}},
        std::array<Indice, 6>{0, 1, 2, 2, 3, 0}};
  }

  uint16_t Renderer::who_is_at(glm::ivec2 screen_position) {
    // Converting the screen position to the viewport (e.g. 322x182) space
    const glm::vec2 ratio = glm::vec2(CONSTANTS::RENDER_WIDTH - 2, CONSTANTS::RENDER_HEIGHT - 2) / glm::vec2(m_window.width(), m_window.height());
    const glm::ivec2 texture_position = glm::ivec2(glm::vec2(screen_position) * ratio) + glm::ivec2(1, 1);

    const int heavy = m_blit_data[(texture_position.y * CONSTANTS::RENDER_WIDTH + texture_position.x) * 4];
    const int remainder = m_blit_data[(texture_position.y * CONSTANTS::RENDER_WIDTH + texture_position.x) * 4 + 1];
    return heavy * 255 + remainder;
  }

  void Renderer::set_id(const uint16_t id) const {
    // Decomposition in base 255....
    const int heavy = id / 255;
    const int remainder = id % 255;
    const float data[4] = {heavy / 255.0f, remainder / 255.0f, 0.0f, 1.0f};
    m_id_uniform.set_data(&data[0]);
  }

  bgfx::ViewId Renderer::begin_frame() const {
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);
    bgfx::setViewName(MAIN_VIEW, "Main pass");
    bgfx::setViewRect(MAIN_VIEW, 0, 0, CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT);
    bgfx::setViewFrameBuffer(MAIN_VIEW, m_framebuffer);

    bgfx::setViewClear(MAIN_VIEW, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);

    ImGui_Implbgfx_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    return MAIN_VIEW;
  }

  void Renderer::end_frame() {
    bgfx::setState(BGFX_STATE_WRITE_RGB);
    bgfx::setViewName(UPSCALE_VIEW, "Upscale Pass");
    bgfx::setViewRect(UPSCALE_VIEW, 0, 0, m_window.width(), m_window.height());
    bgfx::setViewFrameBuffer(UPSCALE_VIEW, BGFX_INVALID_HANDLE);

    // The idea is the map the uv range (0,1) to (ONE_PIXEL, 1.0f - 2.0f*ONE_PIXEL) so we render the (1, 1, 320, 180) cutout
    // We then get a 1 pixel padding around the whole image, that we use with shift
    // texel_errors represents this cutout, it's a rect (x, y, w, h) in uv range instead of pixel range
    constexpr auto uv_one_pixel = glm::vec2(1.0f / CONSTANTS::RENDER_WIDTH, 1.0f / CONSTANTS::RENDER_WIDTH);
    float texel_errors[4] = {uv_one_pixel.x, uv_one_pixel.y, 1.0f - (2.0f * uv_one_pixel.x), 1.0f - (2.0f * uv_one_pixel.y)};

    const glm::vec2 shift = m_camera.get_shift();
    texel_errors[0] += shift.x;
    texel_errors[1] += shift.y;

    m_texel_error.set_data(&texel_errors[0]);

    m_low_resolution_sampler.bind_texture(m_framebuffer_color);
    m_fullscreen_quad.render(UPSCALE_VIEW, m_upscale_program);

    ImGui::Render();
    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());

    bgfx::blit(UPLOAD_VIEW, m_cpu_side_id_texture, 0, 0, m_framebuffer_id);
    bgfx::readTexture(m_cpu_side_id_texture, m_blit_data);
  }
} // namespace lm
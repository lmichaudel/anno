#include "gfx/renderer.hpp"

#include "maths/maths.hpp"
#include "util/constants.hpp"

#include "backends/imgui_impl_glfw.h"
#include "bgfx/bgfx.h"
#include "imgui/imgui_impl_bgfx.h"
#include "log/log.hpp"

// TODO
// Support mutiple lights
// Support directional & omni lights
// DEFERRED RENDERING
// SHADOWS

namespace lm {

  Renderer::Renderer(Window& window, Camera& camera) : m_window(window), m_camera(camera) {
    // m_cpu_side_id_texture = Texture{CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT, bgfx::TextureFormat::RGBA16F, 0 | BGFX_TEXTURE_BLIT_DST | BGFX_TEXTURE_READ_BACK | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP};

    m_gbuffer_normal = Texture{CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT, bgfx::TextureFormat::RGBA16F, 0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT};
    m_gbuffer_position = Texture{CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT, bgfx::TextureFormat::RGBA16F, 0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT};
    m_gbuffer_albedo = Texture{CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT, bgfx::TextureFormat::RGBA8, 0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT};
    m_gbuffer_depth = Texture{CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT, bgfx::TextureFormat::D24S8, 0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT};
    m_gbuffer = Framebuffer{std::vector{&m_gbuffer_position, &m_gbuffer_normal, &m_gbuffer_albedo, &m_gbuffer_depth}};

    m_main_framebuffer_color = Texture{CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT, bgfx::TextureFormat::RGBA8, 0 | BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT};
    m_main_framebuffer = Framebuffer{std::vector{&m_main_framebuffer_color}};

    m_light_program = {"light"};
    m_light_program.add_sampler("g_position", 0);
    m_light_program.add_sampler("g_normal", 1);
    m_light_program.add_sampler("g_albedo", 2);
    m_light_program.add_uniform("u_camera_position", bgfx::UniformType::Vec4);

    m_upscale_program = {"upscale"};
    m_upscale_program.add_sampler("s_main_framebuffer_color", 0);
    m_upscale_program.add_uniform("u_camera_shift", bgfx::UniformType::Enum::Vec4);

    m_fullscreen_quad = {
        std::array{
            Vertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            Vertex{{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            Vertex{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            Vertex{{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}},
        std::array<Indice, 6>{0, 1, 2, 2, 3, 0}};
  }

  void Renderer::render_mesh(const Mesh& mesh, const Program& program, glm::mat4 model, const uint16_t id) const {
    mesh.bind();

    bgfx::setTransform(&model[0][0]);

    if (id != 0) {
      const float data[4] = {static_cast<float>(id) / 255.0f, 0.0f, 0.0f, 0.0f};
      program.set_uniform("u_pick_id", &data[0]);
    }

    bgfx::submit(MAIN_VIEW, program);
  }

  uint16_t Renderer::who_is_at(const glm::ivec2 screen_position) const {
    // Converting the screen position to the viewport (e.g. 322x182) space
    const glm::vec2 ratio = glm::vec2(CONSTANTS::RENDER_WIDTH - 2, CONSTANTS::RENDER_HEIGHT - 2) / glm::vec2(m_window.width(), m_window.height());
    const glm::ivec2 texture_position = glm::ivec2(glm::vec2(screen_position) * ratio) + glm::ivec2(1, 1);

    return m_blit_data[(texture_position.y * CONSTANTS::RENDER_WIDTH + texture_position.x) * 4 + 3];
  }

  void Renderer::begin_frame() const {
    // Step 1 : G-Buffer
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);
    bgfx::setViewName(MAIN_VIEW, "G-Buffer");
    bgfx::setViewRect(MAIN_VIEW, 0, 0, CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT);
    bgfx::setViewFrameBuffer(MAIN_VIEW, m_gbuffer);
    bgfx::setViewClear(MAIN_VIEW, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);

    ImGui_Implbgfx_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void Renderer::end_frame() {
    // Step 2 : Lights
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS);
    bgfx::setViewName(LIGHT_VIEW, "Light Pass");
    bgfx::setViewRect(LIGHT_VIEW, 0, 0, CONSTANTS::RENDER_WIDTH, CONSTANTS::RENDER_HEIGHT);
    bgfx::setViewFrameBuffer(LIGHT_VIEW, m_main_framebuffer);
    bgfx::setViewClear(LIGHT_VIEW, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);

    const float camera_position[4] = {m_camera.get_eye_position().x, m_camera.get_eye_position().y, m_camera.get_eye_position().z, 1.0f};

    // Render lights....
    // ()

    m_light_program.set_uniform("u_camera_position", &camera_position[0]);
    m_light_program.set_sampler("g_position", m_gbuffer_position);
    m_light_program.set_sampler("g_normal", m_gbuffer_normal);
    m_light_program.set_sampler("g_albedo", m_gbuffer_albedo);

    m_fullscreen_quad.bind();
    bgfx::submit(LIGHT_VIEW, m_light_program);

    // Step 3 : Upscale
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

    m_fullscreen_quad.bind();

    m_upscale_program.set_sampler("s_main_framebuffer_color", m_main_framebuffer_color);
    m_upscale_program.set_uniform("u_camera_shift", &texel_errors[0]);
    bgfx::submit(UPSCALE_VIEW, m_upscale_program);

    ImGui::Render();
    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());

    // TODO
    // bgfx::blit(UPLOAD_VIEW, m_cpu_side_id_texture, 0, 0, m_gbuffer_position); // Its stored in the alpha channel :)
    // bgfx::readTexture(m_cpu_side_id_texture, m_blit_data);

    bgfx::frame();
  }
} // namespace lm
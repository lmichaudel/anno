#pragma once

#include "gfx/framebuffer.hpp"
#include "gfx/mesh.hpp"
#include "gfx/program.hpp"
#include "gfx/uniform.hpp"
#include "platform/window.hpp"

static constexpr size_t LOW_RESOLUTION_WIDTH = 320;
static constexpr size_t LOW_RESOLUTION_HEIGHT = 180;
static constexpr double ASPECT = static_cast<double>(LOW_RESOLUTION_WIDTH) / static_cast<double>(LOW_RESOLUTION_HEIGHT);

static constexpr std::string_view UPSCALE_VERTEX_SHADER_PATH = "upscale.vs.sc.bin";
static constexpr std::string_view UPSCALE_FRAGMENT_SHADER_PATH = "upscale.fs.sc.bin";

namespace lm {
  class Renderer {
    static constexpr bgfx::ViewId MAIN_VIEW_ID = 0;
    static constexpr bgfx::ViewId UPSCALE_VIEW_ID = 1;

    Window& m_window;
    // constexpr uint64_t FLAGS = BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT;
    Framebuffer m_framebuffer{LOW_RESOLUTION_WIDTH, LOW_RESOLUTION_HEIGHT, bgfx::TextureFormat::RGBA8, true, bgfx::TextureFormat::D24S8};
    Program m_upscale_program{UPSCALE_VERTEX_SHADER_PATH, UPSCALE_FRAGMENT_SHADER_PATH};
    Uniform m_low_resolution_sampler{"s_albedo", bgfx::UniformType::Enum::Sampler};
    Mesh m_fullscreen_quad{
        std::array{
            Vertex{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            Vertex{{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            Vertex{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            Vertex{{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}
        },
        std::array<Indice, 6>{0, 1, 2, 2, 3, 0}
    };

    public:
      Renderer(Window& window) : m_window(window) {
      }

      ~Renderer() = default;

      Renderer(const Renderer&) = delete;
      Renderer& operator=(const Renderer&) = delete;

      bgfx::ViewId begin_frame() const;
      void end_frame() const;
  };
}
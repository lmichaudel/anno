#pragma once

#include "texture.hpp"

#include <bgfx/bgfx.h>

namespace lm {
  class Framebuffer {
      Texture m_color{};
      Texture m_depth{};
      bgfx::FrameBufferHandle m_handle{bgfx::kInvalidHandle};

    public:
      Framebuffer() = default;
      ~Framebuffer();
      Framebuffer(uint16_t width, uint16_t height, bgfx::TextureFormat::Enum color_format, bool use_depth, bgfx::TextureFormat::Enum depth_format);

      Framebuffer(const Framebuffer&) = delete;
      Framebuffer& operator=(const Framebuffer&) = delete;

      Framebuffer(Framebuffer&& other) noexcept;
      Framebuffer& operator=(Framebuffer&& other) noexcept;

      operator bgfx::FrameBufferHandle() const;

      const Texture& get_color_texture() const { return m_color; }
      const Texture& get_depth_texture() const { return m_depth; }
  };
} // namespace lm
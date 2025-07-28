#pragma once

#include <string_view>
#include <bgfx/bgfx.h>

namespace lm {
  class Texture {
    bgfx::TextureHandle m_handle{bgfx::kInvalidHandle};

    public:
      Texture() = default;
      ~Texture();
      explicit Texture(std::string_view filename);
      Texture(uint16_t width, uint16_t height, bgfx::TextureFormat::Enum format, uint64_t flags = BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT);

      Texture(const Texture&) = delete;
      Texture& operator=(const Texture&) = delete;

      Texture(Texture&& other) noexcept;
      Texture& operator=(Texture&& other) noexcept;

      operator bgfx::TextureHandle() const;
  };
}
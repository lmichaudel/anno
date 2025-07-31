#pragma once

#include <bgfx/bgfx.h>
#include <string_view>

namespace lm {
  class Texture {
      bgfx::TextureHandle m_handle{bgfx::kInvalidHandle};

    public:
      Texture() = default;
      ~Texture();
      Texture(std::string_view filename, uint64_t flags);
      Texture(uint16_t width, uint16_t height, bgfx::TextureFormat::Enum format, uint64_t flags = BGFX_SAMPLER_POINT);

      Texture(const Texture&) = delete;
      Texture& operator=(const Texture&) = delete;

      Texture(Texture&& other) noexcept;
      Texture& operator=(Texture&& other) noexcept;

      operator bgfx::TextureHandle() const;
  };
} // namespace lm
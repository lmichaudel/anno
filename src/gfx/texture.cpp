#include "gfx/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"

#include "log/log.hpp"

namespace lm {
  Texture::Texture(std::string_view filename) {
    int width, height, channels;
    stbi_uc* data = stbi_load(filename.data(), &width, &height, &channels, 4); // Force RGBA

    if (!data) {
      printf("Failed to load texture: %s\n", filename.data());
    }

    const bgfx::Memory* mem = bgfx::copy(data, width * height * 4);
    stbi_image_free(data);

    m_handle = bgfx::createTexture2D(
        static_cast<uint16_t>(width),
        static_cast<uint16_t>(height),
        false,
        1,
        bgfx::TextureFormat::RGBA8,
        BGFX_SAMPLER_POINT,
        mem
        );
  }

  Texture::Texture(uint16_t width, uint16_t height, bgfx::TextureFormat::Enum format, uint64_t flags) {
    m_handle = bgfx::createTexture2D(width, height, false, 1, format, flags);
  }

  Texture::~Texture() {
    if (bgfx::isValid(m_handle)) {
      LOG_DEBUG("Destroying texture {}.", m_handle.idx);
      bgfx::destroy(m_handle);
    }
  }

  Texture::operator bgfx::TextureHandle() const {
    return m_handle;
  }

  Texture::Texture(Texture&& other) noexcept : m_handle(other.m_handle) { other.m_handle = {bgfx::kInvalidHandle}; }

  Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
      if (bgfx::isValid(m_handle)) {
        bgfx::destroy(m_handle);
      }

      m_handle = other.m_handle;
      other.m_handle.idx = bgfx::kInvalidHandle;
    }
    return *this;
  }
}
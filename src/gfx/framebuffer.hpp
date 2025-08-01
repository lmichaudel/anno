#pragma once

#include "texture.hpp"

#include <bgfx/bgfx.h>
#include <vector>

namespace lm {
  class Framebuffer {
      bgfx::FrameBufferHandle m_handle{bgfx::kInvalidHandle};

    public:
      Framebuffer() = default;
      ~Framebuffer();
      explicit Framebuffer(const std::vector<Texture*>& attachments);

      Framebuffer(const Framebuffer&) = delete;
      Framebuffer& operator=(const Framebuffer&) = delete;

      Framebuffer(Framebuffer&& other) noexcept;
      Framebuffer& operator=(Framebuffer&& other) noexcept;

      operator bgfx::FrameBufferHandle() const;
  };
} // namespace lm
#include "gfx/framebuffer.hpp"

#include <bx/bx.h>

namespace lm {
  Framebuffer::Framebuffer(uint16_t width, uint16_t height, bgfx::TextureFormat::Enum color_format, bool use_depth, bgfx::TextureFormat::Enum depth_format) {
    m_color = Texture{width, height, color_format, BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT};
    if (use_depth) {
      m_depth = Texture{width, height, depth_format, BGFX_TEXTURE_RT | BGFX_SAMPLER_POINT};
      const bgfx::TextureHandle attachments[] = {m_color, m_depth};
      m_handle = bgfx::createFrameBuffer(BX_COUNTOF(attachments), attachments, true);
    } else {
      const bgfx::TextureHandle attachments[] = {m_color};
      m_handle = bgfx::createFrameBuffer(BX_COUNTOF(attachments), attachments, true);
    }
  }

  Framebuffer::~Framebuffer() {
    if (bgfx::isValid(m_handle)) {
      bgfx::destroy(m_handle);
    }
  }

  Framebuffer::operator bgfx::FrameBufferHandle() const {
    return m_handle;
  }
} // namespace lm
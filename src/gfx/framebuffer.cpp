#include "gfx/framebuffer.hpp"

namespace lm {
  Framebuffer::Framebuffer(const std::vector<Texture*>& attachments) {
    std::vector<bgfx::TextureHandle> handles(attachments.size());
    for (int i = 0; i < attachments.size(); ++i) {
      handles[i] = *attachments[i];
    }
    m_handle = bgfx::createFrameBuffer(handles.size(), handles.data());
  }

  Framebuffer::~Framebuffer() {
    if (bgfx::isValid(m_handle)) {
      bgfx::destroy(m_handle);
    }
  }

  Framebuffer::operator bgfx::FrameBufferHandle() const {
    return m_handle;
  }

  Framebuffer::Framebuffer(Framebuffer&& other) noexcept : m_handle(other.m_handle) {
    other.m_handle = {bgfx::kInvalidHandle};
  }

  Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
    if (this != &other) {
      if (bgfx::isValid(m_handle)) {
        bgfx::destroy(m_handle);
      }

      m_handle = other.m_handle;
      other.m_handle.idx = bgfx::kInvalidHandle;
    }
    return *this;
  }
} // namespace lm
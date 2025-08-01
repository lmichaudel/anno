#include "gfx/uniform.hpp"

#include <cassert>

namespace lm {
  Uniform::Uniform(std::string_view name, bgfx::UniformType::Enum type) {
    m_handle = bgfx::createUniform(name.data(), type);
    m_type = type;
  }

  Uniform::~Uniform() {
    if (bgfx::isValid(m_handle)) {
      bgfx::destroy(m_handle);
    }
  }

  Uniform::operator bgfx::UniformHandle() const {
    return m_handle;
  }

  void Uniform::bind_texture(const Texture& texture, int stage) const {
    assert(m_type == bgfx::UniformType::Sampler);
    bgfx::setTexture(stage, m_handle, texture);
  }

  void Uniform::set_data(const void* value, int count) const {
    assert(m_type != bgfx::UniformType::Sampler);
    bgfx::setUniform(m_handle, value, count);
  }

  Uniform::Uniform(Uniform&& other) noexcept : m_handle(other.m_handle), m_type(other.m_type) { other.m_handle = {bgfx::kInvalidHandle}; }

  Uniform& Uniform::operator=(Uniform&& other) noexcept {
    if (this != &other) {
      if (bgfx::isValid(m_handle)) {
        bgfx::destroy(m_handle);
      }

      m_handle = other.m_handle;
      m_type = other.m_type;

      other.m_handle = {bgfx::kInvalidHandle};
    }
    return *this;
  }
} // namespace lm
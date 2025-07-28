#pragma once

#include "texture.hpp"

#include <string_view>
#include <bgfx/bgfx.h>

namespace lm {
  class Uniform {
    bgfx::UniformHandle m_handle{bgfx::kInvalidHandle};
    bgfx::UniformType::Enum m_type;

    public:
      Uniform() = default;
      ~Uniform();
      Uniform(std::string_view name, bgfx::UniformType::Enum type);

      Uniform(const Uniform&) = delete;
      Uniform& operator=(const Uniform&) = delete;

      operator bgfx::UniformHandle() const;

      void bind_texture(const Texture& texture, int stage = 0) const;
      void set_data(const void* value, int count = 1) const;
  };
}
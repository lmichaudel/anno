#pragma once

#include "uniform.hpp"

#include <bgfx/bgfx.h>
#include <string>
#include <string_view>
#include <unordered_map>

namespace lm {
  class Program {
      bgfx::ProgramHandle m_handle{bgfx::kInvalidHandle};

      std::unordered_map<std::string, Uniform> m_uniforms{};
      std::unordered_map<std::string, std::pair<Uniform, int>> m_samplers{};

    public:
      Program() = default;
      ~Program();
      Program(std::string_view name);

      Program(const Program&) = delete;
      Program& operator=(const Program&) = delete;

      Program(Program&& other) noexcept;
      Program& operator=(Program&& other) noexcept;

      void add_uniform(std::string identifier, bgfx::UniformType::Enum type);
      void add_sampler(std::string identifier, int stage);

      void set_uniform(std::string_view identifier, const void* data) const;
      void set_sampler(std::string_view identifier, const Texture& texture) const;

      operator bgfx::ProgramHandle() const;
  };
} // namespace lm
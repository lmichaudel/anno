#pragma once

#include <bgfx/bgfx.h>
#include <string_view>

namespace lm {
  class Program {
      bgfx::ProgramHandle m_handle{bgfx::kInvalidHandle};

    public:
      Program() = default;
      ~Program();
      Program(std::string_view name);

      Program(const Program&) = delete;
      Program& operator=(const Program&) = delete;

      Program(Program&& other) noexcept;
      Program& operator=(Program&& other) noexcept;

      operator bgfx::ProgramHandle() const;
  };
} // namespace lm
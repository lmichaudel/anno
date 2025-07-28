#pragma once

#include <string_view>
#include <bgfx/bgfx.h>

namespace lm {
  class Program {
    bgfx::ProgramHandle m_handle{bgfx::kInvalidHandle};

    public:
      Program() = default;
      ~Program();
      Program(std::string_view vs_path, std::string_view fs_path);

      Program(const Program&) = delete;
      Program& operator=(const Program&) = delete;

      Program(Program&& other) noexcept;
      Program& operator=(Program&& other) noexcept;

      operator bgfx::ProgramHandle() const;
  };
}
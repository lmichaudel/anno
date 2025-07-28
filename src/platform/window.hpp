#pragma once

#include <cstdint>

class GLFWwindow;

namespace lm {
  class Window {
    GLFWwindow* m_window = nullptr;

    public:
      Window();
      ~Window();

      bool should_close() const;
      void poll() const;
      std::uint16_t width() const;
      uint16_t height() const;
  };
}
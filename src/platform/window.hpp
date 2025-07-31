#pragma once

#include <cstdint>

#include <GLFW/glfw3.h>

namespace lm {

#define MAX_KEYS GLFW_KEY_LAST + 1

  class Window {
      GLFWwindow* m_window = nullptr;

      bool m_key_down[MAX_KEYS]{};
      bool m_key_pressed[MAX_KEYS]{};
      bool m_key_released[MAX_KEYS]{};

      static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    public:
      Window();
      ~Window();

      bool should_close() const;
      void poll();
      std::uint16_t width() const;
      uint16_t height() const;

      bool is_key_down(int key) const;
      bool was_key_pressed(int key) const;
      bool was_key_released(int key) const;
  };
} // namespace lm
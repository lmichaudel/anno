#pragma once

#include "platform/input.hpp"

namespace lm {
  class Window {
      GLFWwindow* m_window = nullptr;
      Input& m_input;

      static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
      static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
      static void cursor_position_callback(GLFWwindow* window, double x, double y);
      static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    public:
      explicit Window(Input& input);
      ~Window();

      bool should_close() const;
      void poll() const;
      std::uint16_t width() const;
      uint16_t height() const;
  };
} // namespace lm
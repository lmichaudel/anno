#pragma once

#include "maths/maths.hpp"

#include <GLFW/glfw3.h>

#define MAX_KEYS GLFW_KEY_LAST + 1
#define MAX_BUTTONS GLFW_MOUSE_BUTTON_LAST + 1

namespace lm {
  class Input {
      friend class Window;

      bool m_key_down[MAX_KEYS]{};
      bool m_key_pressed[MAX_KEYS]{};
      bool m_key_released[MAX_KEYS]{};

      bool m_button_down[MAX_BUTTONS]{};
      bool m_button_pressed[MAX_BUTTONS]{};
      bool m_button_released[MAX_BUTTONS]{};

      glm::vec2 m_last_cursor_position{0.0f};
      glm::vec<2, int> m_cursor_delta{0};

      void key_callback(int key, int scancode, int action, int mods);
      void cursor_position_callback(double x, double y);
      void mouse_button_callback(int button, int action, int mods);
      void poll();

    public:
      Input();
      ~Input() = default;

      bool is_key_down(int key) const;
      bool was_key_pressed(int key) const;
      bool was_key_released(int key) const;

      bool is_button_down(int button) const;
      bool was_button_pressed(int button) const;
      bool was_button_released(int button) const;

      glm::vec2 get_cursor_position() const;
      glm::vec2 get_cursor_delta() const;
  };
} // namespace lm
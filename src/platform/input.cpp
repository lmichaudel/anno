#include "platform/input.hpp"

#include "log/log.hpp"

#include <cstring>
#include <iostream>

namespace lm {
  void Input::key_callback(const int key, const int scancode, const int action, const int mods) {
    if (key < 0 || key >= MAX_KEYS) {
      LOG_DEBUG("Unhandled key code %d", key);
      return;
    }

    if (action == GLFW_PRESS) {
      m_key_pressed[key] = true;
      m_key_down[key] = true;
    } else if (action == GLFW_RELEASE) {
      m_key_released[key] = true;
      m_key_down[key] = false;
    }
  }

  void Input::mouse_button_callback(const int button, const int action, const int mods) {
    if (button < 0 || button >= MAX_BUTTONS) {
      LOG_DEBUG("Unhandled mouse button {}", button);
      return;
    }

    if (action == GLFW_PRESS) {
      m_button_pressed[button] = true;
      m_button_down[button] = true;
    } else if (action == GLFW_RELEASE) {
      m_button_released[button] = true;
      m_button_down[button] = false;
    }
  }

  void Input::cursor_position_callback(const double x, const double y) {
    const auto new_cursor_position = glm::ivec2(static_cast<int>(x), static_cast<int>(y));
    const auto delta = new_cursor_position - m_last_cursor_position;
    m_cursor_delta = glm::ivec2(delta.x, delta.y);
    m_last_cursor_position = new_cursor_position;
  }

  void Input::poll() {
    m_cursor_delta = {0.0f, 0.0f};
    memset(m_key_pressed, 0, sizeof(m_key_pressed));
    memset(m_key_released, 0, sizeof(m_key_released));
    memset(m_button_pressed, 0, sizeof(m_button_pressed));
    memset(m_button_released, 0, sizeof(m_button_released));
  }

  Input::Input() {
    poll();
  }

  bool Input::is_key_down(const int key) const {
    return key >= 0 && key < MAX_KEYS && m_key_down[key];
  }

  bool Input::was_key_pressed(const int key) const {
    return key >= 0 && key < MAX_KEYS && m_key_pressed[key];
  }

  bool Input::was_key_released(const int key) const {
    return key >= 0 && key < MAX_KEYS && m_key_released[key];
  }

  bool Input::is_button_down(const int button) const {
    return button >= 0 && button < MAX_BUTTONS && m_button_down[button];
  }

  bool Input::was_button_pressed(const int button) const {
    return button >= 0 && button < MAX_BUTTONS && m_button_pressed[button];
  }

  bool Input::was_button_released(const int button) const {
    return button >= 0 && button < MAX_BUTTONS && m_button_released[button];
  }

  glm::ivec2 Input::get_cursor_position() const {
    return m_last_cursor_position;
  }

  glm::ivec2 Input::get_cursor_delta() const {
    return m_cursor_delta;
  }
} // namespace lm
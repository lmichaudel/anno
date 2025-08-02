#include "camera.hpp"

#include "util/constants.hpp"

#include <bgfx/bgfx.h>

namespace lm {
  constexpr auto FORWARD = glm::vec3(0.0f, 0.0f, 1.0f);
  constexpr auto RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
  constexpr auto UP = glm::vec3(0.0f, 1.0f, 0.0f);

  void Camera::compute_view_proj() {
    const auto eye = m_pivot + forward() * m_distance;

    // This ensures we KEEP the same texel alignment, effectively fixing jittering/shimmering
    const auto snapped_position = m_inverse_rotation_matrix * eye - m_last_rotation_position;
    const auto clamped_position = glm::vec3(round(snapped_position.x / m_texel_size) * m_texel_size, round(snapped_position.y / m_texel_size) * m_texel_size, 0.0f);
    const auto error = clamped_position - snapped_position;

    m_shift = glm::vec2(-error.x / CONSTANTS::RENDER_WIDTH, error.y / CONSTANTS::RENDER_HEIGHT) / m_texel_size;

    m_view = glm::lookAt(eye, m_pivot, glm::vec3(0.0f, 1.0f, 0.0f));
    m_proj = glm::ortho(-m_size * CONSTANTS::ASPECT / 2.0f, m_size * CONSTANTS::ASPECT / 2.0f, -m_size / 2.0f, m_size / 2.0f, -1000.0f, 1000.0f);
    m_proj = glm::translate(m_proj, -glm::vec3(error.x, error.y, 0.0f));
  }

  void Camera::bind() const {
    bgfx::setViewTransform(0, &m_view[0][0], &m_proj[0][0]);
  }

  glm::vec3 Camera::forward() const {
    return m_local_forward;
  }

  glm::vec3 Camera::right() const {
    return m_local_right;
  }

  glm::vec3 Camera::up() const {
    return m_local_up;
  }

  float Camera::unit_pixel() const {
    return m_texel_size;
  }

  void Camera::set_pivot_position(const glm::vec3 position) {
    if (position == m_pivot) {
      return;
    }

    m_pivot = position;
    compute_view_proj();
  }

  glm::vec3 Camera::get_pivot_position() const {
    return m_pivot;
  }

  void Camera::set_rotation(const float rotation) {
    if (rotation == m_rotation) {
      return;
    }

    m_rotation = rotation;

    const auto angles = glm::vec3(glm::radians(-30.0f), glm::radians(m_rotation), 0.0f);

    // Do we need to orthonormalize the matrix ? My guess is that a quaternion is of course already orthonormalized...
    m_rotation_matrix = glm::mat3(glm::quat(angles));
    m_inverse_rotation_matrix = glm::inverse(m_rotation_matrix);
    m_last_rotation_position = m_pivot;

    m_local_forward = glm::normalize(m_rotation_matrix * FORWARD);
    m_local_right = glm::normalize(m_rotation_matrix * RIGHT);
    m_local_up = glm::normalize(m_rotation_matrix * UP);

    compute_view_proj();
  }

  float Camera::get_rotation() const {
    return m_rotation;
  }

  void Camera::set_distance(const float distance) {
    if (distance == m_distance) {
      return;
    }

    m_distance = distance;
    compute_view_proj();
  }

  float Camera::get_distance() const {
    return m_distance;
  }

  void Camera::set_size(const float size) {
    if (size == m_size) {
      return;
    }

    m_size = size;
    m_texel_size = m_size / CONSTANTS::RENDER_HEIGHT;
    compute_view_proj();
  }

  float Camera::get_size() const {
    return m_size;
  }

  glm::vec2 Camera::get_shift() const {
    return m_shift;
  }

  glm::vec3 Camera::get_eye_position() const {
    return m_pivot + forward() * m_distance;
  }
} // namespace lm
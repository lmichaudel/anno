#pragma once

#include "maths/maths.hpp"

namespace lm {

  class Camera {
      glm::vec3 m_pivot;
      float m_size;
      float m_rotation;
      float m_distance;

      glm::vec3 m_local_forward;
      glm::vec3 m_local_right;
      glm::vec3 m_local_up;

      glm::mat3 m_rotation_matrix;
      glm::mat3 m_inverse_rotation_matrix;
      glm::vec3 m_last_rotation_position;

      glm::vec2 m_shift;

      float m_texel_size;

      glm::mat4 m_view;
      glm::mat4 m_proj;

      void compute_view_proj();

    public:
      Camera() {
        set_pivot_position(glm::vec3(0.0f, 0.0f, 0.0f));
        set_size(20.0f);
        set_rotation(45.0f);
        set_distance(20.0f);
      }

      void bind() const;

      glm::vec3 forward() const;
      glm::vec3 right() const;
      glm::vec3 up() const;

      float unit_pixel() const;

      glm::vec2 get_shift() const;

      void set_pivot_position(glm::vec3 position);
      glm::vec3 get_pivot_position() const;

      void set_rotation(float rotation);
      float get_rotation() const;

      void set_distance(float distance);
      float get_distance() const;

      void set_size(float size);
      float get_size() const;
  };
} // namespace lm
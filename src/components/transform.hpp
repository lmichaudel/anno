#pragma once

#include "bx/math.h"

#include <glm/vec3.hpp>

namespace lm {
  struct Transform {
      void set_position(const glm::vec3 position) {
        m_position = position;
        recalculate_mtx();
      }

      void set_rotation(const glm::vec3 rotation) {
        m_rotation = rotation;
        recalculate_mtx();
      }

      void set_scale(const glm::vec3 scale) {
        m_scale = scale;
        recalculate_mtx();
      }

      glm::vec3 get_position() const { return m_position; }
      glm::vec3 get_rotation() const { return m_rotation; }
      glm::vec3 get_scale() const { return m_scale; }
      float* get_matrix() { return m_mtx.data(); }

    private:
      glm::vec3 m_position{0.0f};
      glm::vec3 m_rotation{0.0f};
      glm::vec3 m_scale{1.0f};

      std::array<float, 16> m_mtx{};

      void recalculate_mtx() {
        bx::mtxTranslate(m_mtx.data(), m_position.x, m_position.y, m_position.z);
        bx::mtxRotateXYZ(m_mtx.data(), m_rotation.x, m_rotation.y, m_rotation.z);
        bx::mtxScale(m_mtx.data(), m_scale.x, m_scale.y, m_scale.z);
      }
  };
} // namespace lm
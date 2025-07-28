#pragma once

#include "core/transform.hpp"

namespace lm {
  class Object {
    Transform m_transform;

    public:
      virtual ~Object() = default;

      void set_position(const glm::vec3& position) {
        m_transform.position = position;
        on_position_changed();
      }

      void set_rotation(const glm::vec3& rotation) {
        m_transform.rotation = rotation;
        on_rotation_changed();
      }

      void set_scale(const glm::vec3& scale) {
        m_transform.scale = scale;
        on_scale_changed();
      }

    protected:
      virtual void init() {
      }

      virtual void update() {
      }

      virtual void tick(float dt) {
      }

      virtual void draw() {
      }

      virtual void on_position_changed() {
      }

      virtual void on_rotation_changed() {
      }

      virtual void on_scale_changed() {
      }
  };
}
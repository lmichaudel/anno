#include "camera.hpp"

#include "bx/math.h"
#include "gfx/renderer.hpp"

namespace lm {
  Camera::Camera() {
    recalculate_view();
    recalculate_proj();
  }

  void Camera::set_zoom_level(float level) {
    m_zoom_level = level;
  }

  void Camera::on_position_changed() {
    recalculate_view();
  }

  void Camera::recalculate_view() {
    bx::mtxLookAt(mtxView,
                  bx::Vec3(0.0f, 0.0f, -5.0f), // eye
                  bx::Vec3(0.0f, 0.0f, 0.0f), // at
                  bx::Vec3(0.0f, 1.0f, 0.0f) // up
        );
  }

  void Camera::recalculate_proj() {
    bx::mtxOrtho(mtxProj, -m_zoom_level * ASPECT, m_zoom_level * ASPECT, -m_zoom_level, m_zoom_level,
                 0.1f, 1000.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
  }

  void Camera::bind() {
    bgfx::setViewTransform(0, mtxView, mtxProj);
  }

}
#pragma once

#include "core/object.hpp"

namespace lm {
  class Camera : public Object {
    float mtxView[16]{};
    float mtxProj[16]{};

    float m_zoom_level = 2.0f;

    protected:
      void on_position_changed() override;

    public:
      Camera();

      void set_zoom_level(float level);

      void recalculate_view();
      void recalculate_proj();

      void bind();
  };
}
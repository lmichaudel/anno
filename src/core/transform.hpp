#pragma once

#include <glm/vec3.hpp>

namespace lm {
  struct Transform {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
  };
}
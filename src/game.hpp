#pragma once

#include "core/engine.hpp"

namespace lm {
  class Game {
      Engine& engine;

      Mesh cube, plane;

      Texture texture{"res/textures/texture.jpg"};
      Texture rock{"res/textures/rock.jpg"};

      Program program;

      float light_dir[4] = {-1.0f, -1.5f, -1.0f, 0.0f};

    public:
      explicit Game(Engine& engine) : engine(engine) {}
      void init();

      void update(float dt);
      void tick(float dt);
      void render(float dt) const;
  };
} // namespace lm
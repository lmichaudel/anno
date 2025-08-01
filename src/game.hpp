#pragma once

#include "core/engine.hpp"

namespace lm {
  class Game {
      Engine& engine;

      Mesh cube, plane;

      Uniform s_texture{"s_texture", bgfx::UniformType::Sampler};
      Uniform u_light_dir{"lightDir", bgfx::UniformType::Vec4};

      Texture texture{"res/textures/texture.jpg"};
      Texture rock{"res/textures/rock.jpg"};

      Program program{"cube"};

      float light_dir[4] = {-1.0f, -1.5f, -1.0f, 0.0f};

    public:
      Game(Engine& engine) : engine(engine) {}
      void init();

      void update(float dt);
      void tick(float dt);
      void render(float dt) const;
  };
} // namespace lm
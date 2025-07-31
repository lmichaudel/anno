#pragma once

#include "gfx/primitive.hpp"
#include "gfx/renderer.hpp"

namespace lm {
  struct GameState {
      const uint64_t texture_flags = BGFX_SAMPLER_POINT;
      Mesh cube, plane, cylinder;
      Uniform s_texture{"s_texture", bgfx::UniformType::Sampler};
      Uniform u_light_dir{"lightDir", bgfx::UniformType::Vec4};
      Texture texture{"res/textures/texture.jpg", texture_flags};
      Texture rock{"res/textures/rock.jpg", texture_flags};
      Texture checker{"res/textures/checker.png", texture_flags};
      Program program{"cube.vs.sc.bin", "cube.fs.sc.bin"};

      float lightDir[4] = {-1.0f, -1.5f, -1.0f, 0.0f};

      GameState() {
        cube = {primitives::cube_vertices, primitives::cube_indices};
        plane = {primitives::plane_vertices, primitives::plane_indices};
        cylinder = {primitives::cylinder_vertices,
                    primitives::cylinder_indices};
      }

      void update(float dt);
      void tick(float dt);
      void draw(float dt);
  };
} // namespace lm
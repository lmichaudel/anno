#pragma once

#include "gfx/renderer.hpp"

namespace lm {
  struct GameState {
    Mesh cube;
    Uniform s_texture{"s_texture", bgfx::UniformType::Sampler};
    Uniform u_light_dir{"lightDir", bgfx::UniformType::Vec4};
    Texture texture{"res/textures/texture.jpg"};
    Program program{"cube.vs.sc.bin", "cube.fs.sc.bin"};

    float mtxModel[16]{};
    float ax{0};
    float ay{0};

    float lightDir[4] = {-1.0f, -1.5f, -1.0f, 0.0f};

    GameState() {
      cube = {std::array{
                  Vertex{{-1, -1, 1}, {0, 0, 1}, {0.0f, 0.0f}},
                  Vertex{{1, -1, 1}, {0, 0, 1}, {1.0f, 0.0f}},
                  Vertex{{1, 1, 1}, {0, 0, 1}, {1.0f, 1.0f}},
                  Vertex{{-1, 1, 1}, {0, 0, 1}, {0.0f, 1.0f}},
                  Vertex{{-1, -1, -1}, {0, 0, -1}, {1.0f, 0.0f}},
                  Vertex{{-1, 1, -1}, {0, 0, -1}, {1.0f, 1.0f}},
                  Vertex{{1, 1, -1}, {0, 0, -1}, {0.0f, 1.0f}},
                  Vertex{{1, -1, -1}, {0, 0, -1}, {0.0f, 0.0f}},
                  Vertex{{-1, 1, -1}, {0, 1, 0}, {0.0f, 1.0f}},
                  Vertex{{-1, 1, 1}, {0, 1, 0}, {0.0f, 0.0f}},
                  Vertex{{1, 1, 1}, {0, 1, 0}, {1.0f, 0.0f}},
                  Vertex{{1, 1, -1}, {0, 1, 0}, {1.0f, 1.0f}},
                  Vertex{{-1, -1, -1}, {0, -1, 0}, {0.0f, 0.0f}},
                  Vertex{{1, -1, -1}, {0, -1, 0}, {1.0f, 0.0f}},
                  Vertex{{1, -1, 1}, {0, -1, 0}, {1.0f, 1.0f}},
                  Vertex{{-1, -1, 1}, {0, -1, 0}, {0.0f, 1.0f}},
                  Vertex{{1, -1, -1}, {1, 0, 0}, {0.0f, 0.0f}},
                  Vertex{{1, 1, -1}, {1, 0, 0}, {0.0f, 1.0f}},
                  Vertex{{1, 1, 1}, {1, 0, 0}, {1.0f, 1.0f}},
                  Vertex{{1, -1, 1}, {1, 0, 0}, {1.0f, 0.0f}},
                  Vertex{{-1, -1, -1}, {-1, 0, 0}, {1.0f, 0.0f}},
                  Vertex{{-1, -1, 1}, {-1, 0, 0}, {0.0f, 0.0f}},
                  Vertex{{-1, 1, 1}, {-1, 0, 0}, {0.0f, 1.0f}},
                  Vertex{{-1, 1, -1}, {-1, 0, 0}, {1.0f, 1.0f}}
              },
              std::array<Indice, 36>{
                  0, 1, 2, 2, 3, 0,
                  4, 5, 6, 6, 7, 4,
                  8, 9, 10, 10, 11, 8,
                  12, 13, 14, 14, 15, 12,
                  16, 17, 18, 18, 19, 16,
                  20, 21, 22, 22, 23, 20
              }};
    }

    void update(double dt);
    void tick(double dt);
    void draw(double dt);
  };
}